/**
 * jsSMS - A Sega Master System/Game Gear emulator in JavaScript
 * Copyright (C) 2012  Guillaume Marty (https://github.com/gmarty)
 * Based on JavaGear Copyright (c) 2002-2008 Chris White
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

'use strict';



/**
 * Default implementation of UI. Could as well be defined as an interface, to
 * make sure we don't forget anything when implementing it.
 *
 * @constructor
 * @param {JSSMS} sms
 */
JSSMS.DummyUI = function(sms) {
  this.main = sms;
  this.reset = function() {
  };
  this.updateStatus = function() {
  };
  this.writeAudio = function() {
  };
  this.writeFrame = function() {
  };
  this.updateDisassembly = function() {
  };
  this.canvasImageData = {
    data: []
  };
};

if (window['$']) {
  /**
   * @constructor
   * @param {Object.<string, Object.<string, string>>} roms A list of rom files.
   */
  $.fn['JSSMSUI'] = function(roms) {
    var parent = /** HTMLElement **/ (this);
    var UI = function(sms) {
      this.main = sms;

      // Exit if ran from Opera Mini.
      if (Object.prototype.toString.call(window['operamini']) === '[object OperaMini]') {
        $(parent).html('<div class="alert alert-error"><strong>¡Oh no!</strong> Tu navegador no soporta este emulador. ¡Prueba con la última versión de Firefox, Google Chrome, Opera o Safari!</div>');
        return;
      }

      var self = this;

      // Create UI
      var root = $('<div></div>');
      var screenContainer = $('<div id="screen"></div>');
      var gamepadContainer = $('<div class="gamepad" id="gamepad"><div class="direction"><div class="upbtn"></div><div class="right"></div><div class="left"></div><div class="down"></div></div><div class="buttons"><div class="start"></div><div class="fire1"></div><div class="fire2"></div></div></div>');
      var controls = $('<div id="controls"></div>');

      // General settings
      /**
       * Contains the fullscreen API prefix or false if not supported.
       * @type {string|boolean}
       */
      var fullscreenSupport = JSSMS.Utils.getPrefix(['fullscreenEnabled', 'mozFullScreenEnabled', 'webkitCancelFullScreen']);

      var requestAnimationFramePrefix = JSSMS.Utils.getPrefix(['requestAnimationFrame', 'msRequestAnimationFrame', 'mozRequestAnimationFrame', 'webkitRequestAnimationFrame'], window);

      var i;

      if (requestAnimationFramePrefix) {
        this.requestAnimationFrame = window[requestAnimationFramePrefix].bind(window);
      } else {
        var lastTime = 0;
        this.requestAnimationFrame = function(callback) {
          var currTime = JSSMS.Utils.getTimestamp();
          var timeToCall = Math.max(0, (1000 / 60) - (currTime - lastTime));
          window.setTimeout(function() {
            lastTime = JSSMS.Utils.getTimestamp();
            callback();
          }, timeToCall);
        };
      }

      // Screen
      this.screen = $('<canvas width=' + '580px' + ' height=' + '580px' + ' moz-opaque></canvas>');
      this.canvasContext = this.screen[0].getContext('2d', {
        'alpha': true// See http://wiki.whatwg.org/wiki/CanvasOpaque
      });

      // Nearest-neighbour rendering for scaling pixel-art.
      this.canvasContext['webkitImageSmoothingEnabled'] = false;
      this.canvasContext['mozImageSmoothingEnabled'] = false;
      this.canvasContext['imageSmoothingEnabled'] = false;

      if (!this.canvasContext.getImageData) {
        $(parent).html('<div class="alert alert-error"><strong>Oh no!</strong> Tu navegador no soporta la escritura de píxeles directamente en la etiqueta <code>&lt;canvas&gt;</code>. ¡Prueba con la última versión de Firefox, Google Chrome, Opera o Safari!</div>');
        return;
      }

      this.canvasImageData = this.canvasContext.getImageData(0, 0, SMS_WIDTH, SMS_HEIGHT);

      // Gamepad
      this.gamepad = {
        up: P1_KEY_UP,
        down: P1_KEY_DOWN,
        left: P1_KEY_LEFT,
        right: P1_KEY_RIGHT,
        fire1: P1_KEY_FIRE1,
        fire2: P1_KEY_FIRE2
      };
      var startButton = $('.start', gamepadContainer);

      // Rom selector
      this.romContainer = $('<div id="romSelector"></div>');
      this.romSelect = $('<button class="btn-transparent monBotton"></button>')
        .on('click', function() {
          self.loadROM();
        });

      // Buttons
      this.buttons = Object.create(null);

      this.buttons.start = $('<input type="button" value="Iniciar" class="btn btn-danger" >')
        .click(function() {
          if (!self.main.isRunning) {
            self.main.start();
            self.buttons.start.attr('value', 'Pausa');
          } else {
            self.main.stop();
            self.updateStatus('Paused');
            self.buttons.start.attr('value', 'Seguir');
          }
        });

      this.buttons.reset = $('<input type="button" value="Reiniciar" class="btn" disabled="disabled">')
        .click(function() {
          if (!self.main.reloadRom()) {
            $(this).attr('disabled', 'disabled');
            return;
          }
          self.main.reset();
          self.main.vdp.forceFullRedraw();
          self.main.start();
        });

      if (ENABLE_DEBUGGER) {
        this.dissambler = $('<div id="dissambler"></div>');
        $(parent).after(this.dissambler);
        this.buttons.nextStep = $('<input type="button" value="Next step" class="btn" disabled="disabled">')
          .click(function() {
            self.main.nextStep();
          });
      }

      if (this.main.soundEnabled) {
        this.buttons.sound = $('<input type="button" value="Activar sonido" class="btn" disabled="disabled">')
          .click(function() {
            if (self.main.soundEnabled) {
              self.main.soundEnabled = false;
              self.buttons.sound.attr('value', 'Activar sonido');
            } else {
              self.main.soundEnabled = true;
              self.buttons.sound.attr('value', 'Desactivar sonido');
            }
          });
      }

      if (fullscreenSupport) {
        // @todo Add an exit fullScreen button.
        this.buttons.fullscreen = $('<input type="button" value="Pantalla completa" class="btn">')
          .click(function() {
            var screen = /** @type {HTMLDivElement} */ (screenContainer[0]);

            if (screen.requestFullscreen) {
              screen.requestFullscreen();
            } else if (screen.mozRequestFullScreen) {
              screen.mozRequestFullScreen();
            } else {
              screen.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
            }
          });
      } else {
        this.zoomed = false;

        this.buttons.zoom = $('<input type="button" value="Zoom in" class="btn hidden-phone">')
          .click(function() {
            if (self.zoomed) {
              self.screen.animate({
                width: SMS_WIDTH + 'px',
                height: SMS_HEIGHT + 'px'
              }, function() {
                $(this).removeAttr('style');
              });
              self.buttons.zoom.attr('value', 'Zoom in');
            } else {
              self.screen.animate({
                width: (SMS_WIDTH * 2) + 'px',
                height: (SMS_HEIGHT * 2) + 'px'
              });
              self.buttons.zoom.attr('value', 'Zoom out');
            }
            self.zoomed = !self.zoomed;
          });
      }

      // Software buttons - touch
      gamepadContainer.on('touchstart touchmove', function(evt) {
        evt.preventDefault();

        self.main.keyboard.controller1 = 0xFF;

        var touches = evt.originalEvent.touches;

        for (var i = 0; i < touches.length; i++) {
          var target = document.elementFromPoint(touches[i].clientX, touches[i].clientY);

          if (!target) {
            continue;
          }

          var className = target.className;

          if (!className || !self.gamepad[className]) {
            continue;
          }

          var key = self.gamepad[className];
          self.main.keyboard.controller1 &= ~key;
        }
      });

      gamepadContainer.on('touchend', function(evt) {
        evt.preventDefault();

        if (evt.originalEvent.touches.length === 0) {
          self.main.keyboard.controller1 = 0xFF;
        }
      });

      // Software buttons - click
      function mouseDown(evt) {
        var className = this.className;
        var key = self.gamepad[className];
        self.main.keyboard.controller1 &= ~key;
        evt.preventDefault();
      }

      function mouseUp(evt) {
        var className = this.className;
        var key = self.gamepad[className];
        self.main.keyboard.controller1 |= key;
        evt.preventDefault();
      }

      for (i in this.gamepad) {
        $('.' + i, gamepadContainer)
          .mousedown(mouseDown)
          .mouseup(mouseUp);
      }

      startButton
        .on('mousedown touchstart', function(evt) {
          if (self.main.is_sms) {
            self.main.pause_button = true;       // Pause
          } else {
            self.main.keyboard.ggstart &= ~0x80; // Start
          }
          evt.preventDefault();
        })
        .on('mouseup touchend', function(evt) {
          if (!self.main.is_sms) {
            self.main.keyboard.ggstart |= 0x80;  // Start
          }
          evt.preventDefault();
        });

      // Keyboard
      $(document)
        .bind('keydown', function(evt) {
          self.main.keyboard.keydown(evt);
          //console.log(self.main.keyboard.controller1, self.main.keyboard.ggstart);
        })
        .bind('keyup', function(evt) {
          self.main.keyboard.keyup(evt);
          //console.log(self.main.keyboard.controller1, self.main.keyboard.ggstart);
        });

      // Append buttons to controls div.
      for (i in this.buttons) {
        this.buttons[i].appendTo(controls);
      }

      this.log = $('<div id="status" class="sms-status alert alert-success"></div>');

      this.screen.appendTo(screenContainer);
      gamepadContainer.appendTo(screenContainer);
      screenContainer.appendTo(root);
      this.romContainer.appendTo(root);
      controls.appendTo(root);
      this.log.appendTo(root);
      root.appendTo($(parent));

      if (roms !== undefined) {
        this.setRoms(roms);
      }
    };

    UI.prototype = {
      reset: function() {
        this.screen[0].width = SMS_WIDTH;
        this.screen[0].height = SMS_HEIGHT;

        this.log.empty();

        if (ENABLE_DEBUGGER) {
          this.dissambler.empty();
        }
      },


      /**
       * Given an map of roms, build a <select> tag to allow game selection.
       *
       * @param {Object.<Array.<string>>} roms The list of roms.
       */
      setRoms: function(roms) {
        var groupName,
            optgroup,
            length,
            i,
            count = 0;

        this.romSelect.children().remove();

        for (groupName in roms) {
          if (roms.hasOwnProperty(groupName)) {
            length = roms[groupName].length;
          }
          count++;
        }
        if (count) {
          this.romSelect.appendTo(this.romContainer);
        }
      },


      loadROM: function() {
        var self = this;

        this.updateStatus('Iniciando...');
        $.ajax({
          url: encodeURI(roms.homebrew),
          xhr: function() {
            var xhr = $.ajaxSettings.xhr();
            if (xhr.overrideMimeType !== undefined) {
              // Download as binary
              xhr.overrideMimeType('text/plain; charset=x-user-defined');
            }
            self.xhr = xhr; // ???
            return xhr;
          },
          complete: function(xhr, status) {
            var data;

            if (status === 'error') {
              self.updateStatus('Este juego no puede ser cargado, o no es válido.');
              return;
            }

            /*if (JSSMS.Utils.isIE()) {
             var charCodes = JSNESBinaryToArray(xhr.responseBody).toArray();
             data = String.fromCharCode.apply(undefined, charCodes);
             } else {*/
            data = xhr.responseText;
            //}

            self.main.stop();
            self.main.readRomDirectly(data, self.romSelect.val());
            self.main.reset();
            self.main.vdp.forceFullRedraw();
            self.enable();
          }
        });
      },


      /**
       * Enable and reset UI elements.
       */
      enable: function() {
        /*this.buttons.pause.removeAttr('disabled');
         if (this.main.isRunning) {
         this.buttons.pause.attr('value', 'pause');
         } else {
         this.buttons.pause.attr('value', 'resume');
         }*/
        this.updateStatus('Pulsa en «Iniciar» para iniciar el juego.');
        this.buttons.start.removeAttr('disabled');
        this.buttons.start.attr('value', 'Iniciar');
        this.buttons.reset.removeAttr('disabled');
        if (ENABLE_DEBUGGER) {
          this.buttons.nextStep.removeAttr('disabled');
        }
        if (this.main.soundEnabled) {
          if (this.buttons.sound) {
            this.buttons.sound.attr('value', 'Desactivar sonido');
          } else {
            this.buttons.sound.attr('value', 'Activar sonido');
          }
        }
      },


      /**
       * Update the message. Used mainly for displaying frame rate.
       *
       * @param {string} s The message to display.
       */
      updateStatus: function(s) {
        this.log.text(s);
      },


      /**
       * @param {Array.<number>} buffer
       */
      writeAudio: function(buffer) {
        var source = this.main.audioContext.createBufferSource();
        source.buffer = buffer;
        source.connect(this.main.audioContext.destination);
        source.start();
      },


      /**
       * Update the canvas screen. ATM, prevBuffer is not used. See JSNES for
       * an implementation of differential update.
       */
      writeFrame: function() {
        /**
         * Contains the visibility API prefix or false if not supported.
         * @type {string|boolean}
         */
        var hiddenPrefix = JSSMS.Utils.getPrefix(['hidden', 'mozHidden', 'webkitHidden', 'msHidden']);

        if (hiddenPrefix) {
          // If browser supports visibility API and this page is hidden, we exit.
          return function() {
            if (document[hiddenPrefix]) {
              return;
            }

            this.canvasContext.putImageData(this.canvasImageData, 0, 0);
          };
        } else {
          return function() {
            this.canvasContext.putImageData(this.canvasImageData, 0, 0);
          };
        }
      }(),


      /**
       * A function called at each cpu instruction and display information relative to debug.
       * \@todo If currentAddress is not in this.main.cpu.instructions, then start parsing from it.
       */
      updateDisassembly: function(currentAddress) {
        var startAddress = currentAddress < 8 ? 0 : currentAddress - 8;
        var instructions = this.main.cpu.instructions;
        var length = instructions.length;
        var html = '';
        var i = startAddress;
        var num = 0;

        for (; num < 16 && i <= length; i++) {
          if (instructions[i]) {
            html += '<div' + (instructions[i].address === currentAddress ? ' class="current"' : '') + '>' + instructions[i].hexAddress +
              (instructions[i].isJumpTarget ? ':' : ' ') +
              '<code>' + instructions[i].inst + '</code></div>';

            num++;
          }
        }

        this.dissambler.html(html);
      }
    };

    return UI;
  };
}
