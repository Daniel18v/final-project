        var galaga;
        var canvas;
        var offscreenCanvas;
        var context;
        var offscreenContext;
        var imageData;
        var audio = {};

        var BUFFER_COUNT = 8;
        var BUFFER_SIZE = 1024; // samples
        var _writePos = 4 * BUFFER_SIZE - 960;
        var _lastPlayed = BUFFER_COUNT - 1;
        var _totExecutionTime = .0;
        var _totEmulatedTime = .0;

        function initVideo() {
            // create offscreen canvas
            offscreenCanvas = document.createElement('canvas');
            offscreenCanvas.width = 224;
            offscreenCanvas.height = 288;
            offscreenContext = offscreenCanvas.getContext('2d');
            imageData = offscreenContext.createImageData(224, 288);

            canvas = document.getElementById("myGLCanvas");
            context = canvas.getContext("2d");
            canvas.height = window.innerHeight - 24;
            canvas.width = canvas.height * offscreenCanvas.width / offscreenCanvas.height;
            context.scale(canvas.width / offscreenCanvas.width, canvas.height / offscreenCanvas.height);
        }

        function initAudio() {
            window.AudioContext = window.AudioContext || window.webkitAudioContext;
            if (!window.AudioContext) {
                throw 'Web Audio API is not available!';
            }

            audio.context = new window.AudioContext;
            audio.buffer = new Int16Array(BUFFER_COUNT * BUFFER_SIZE);

            audio.scriptNode = audio.context.createScriptProcessor(BUFFER_SIZE, 0, 1);
            audio.scriptNode.connect(audio.context.destination);

            audio.scriptNode.onaudioprocess = function (audioEvent) {
                try {
                    var channelData = audioEvent.outputBuffer.getChannelData(0);

                    _lastPlayed = (_lastPlayed + 1) % BUFFER_COUNT;
                    var j = _lastPlayed * BUFFER_SIZE;
                    for (var i = 0; i < BUFFER_SIZE; i++, j++) {
                        // Convert from [-32768:32767] to [-1.0:1.0]
                        channelData[i] = audio.buffer[j] / 0x8000;
                        audio.buffer[j] = 0;
                    }
                }
                catch (e) {
                    console.log('Web Audio API error playing back audio: ' + e.toString());
                }
            }
        }

        function main() {
            document.addEventListener("keydown", onKeyDown);
            document.addEventListener("keyup", onKeyUp);

            galaga = new Module.GalagaMachine();
            initVideo();
            initAudio();

            repeatOften();
        }

        var prevTimestamp;
        function repeatOften(timestamp) {
            var interval;
            if (!prevTimestamp || !timestamp) {
                interval = 20.0;
            }
            else {
                interval = timestamp - prevTimestamp;
            }
            prevTimestamp = timestamp;
            step(interval);
            requestAnimationFrame(repeatOften);
        }

        //var shown = false;
        function step(interval) {
            if (interval > 67) {
                interval = 67;
            }

            var tStart = performance.now();
            galaga.Run(interval,
                function (video, audioBuffer) {
                    var tEnd = performance.now();
                    //console.log('run ' + interval + 'ms in actual ' + (t1 - t0) + 'ms');

                    _totExecutionTime += (tEnd - tStart);
                    _totEmulatedTime += interval;
                    //if (_totEmulatedTime > 100000 && ! shown)
                    //{
                    //    alert('run ' + _totEmulatedTime + 'ms in actual ' + _totExecutionTime + 'ms');
                    //    shown = true;
                    //}

                    // Video is a Uint8Array that aliases directly into the Emscripten heap
                    imageData.data.set(video);
                    offscreenContext.putImageData(imageData, 0, 0);
                    context.drawImage(offscreenCanvas, 0, 0);

                    // HTML5 audio is... complicated. Emitting samples with ... is not reliable,
                    // so we are use a ScriptProcessorNode that invokes periodically a callback
                    // (onaudioprocess) to request the next samples to play.
                    // Problem: ScriptProcessorNode works at a fixed sampling rate, which depends
                    // on the audio hardware. This is 44.1KHz most often, but sometimes 48KHz (like
                    // with a Microsoft Surface Pro 3). The Galaga emulator generates audio samples
                    // at 48KHz, so when we run at 44.1KHz we need to resample (by using a very rough
                    // algorithm, that just discards 13 out of 160 samples, no interpolation).

                    if (audio.context.sampleRate == 44100) {
                        // Resample to 44.1KHz (crude resampling: just discards every Nth sample...)
                        var max = BUFFER_SIZE * _lastPlayed;
                        var q = 147.0 / 160.0;
                        var s = .0;
                        var i = 0;
                        var j = _writePos;
                        var len = BUFFER_SIZE * BUFFER_COUNT;
                        while (i < audioBuffer.length) {
                            if ((i % 960) == 0) {
                                s = .0;
                            }
                            s += q;
                            j = (_writePos + Math.floor(s)) % len;
                            if (j == max) {
                                break;
                            }
                            audio.buffer[j] = audioBuffer[i];
                            i++;
                        }
                        _writePos = j;
                    }
                    else // assume 48KHz
                    {
                        var max = BUFFER_SIZE * _lastPlayed;
                        var i = 0;
                        var j = _writePos;
                        var len = BUFFER_SIZE * BUFFER_COUNT;
                        while (i < audioBuffer.length) {
                            if (j == max) {
                                break;
                            }
                            audio.buffer[j] = audioBuffer[i];
                            i++;
                            j = (j + 1) % len;
                        }
                        _writePos = j;
                    }
                });
        }

        function onKeyDown(event) {
            var key = event.keyCode || event.which;
            switch (key) {
                case 48: //"0"
                    galaga.set_InsertCoin(true);
                    break;

                case 49: //"1"
                    galaga.set_Start1Player(true);
                    break;

                case 50: //"2"
                    galaga.set_Start2Player(true);
                    break;

                case 37: //"Left"
                    galaga.set_MoveLeft(true);
                    break;

                case 39: //"Right"
                    galaga.set_MoveRight(true);
                    break;

                case 17: //"Control"
                    galaga.set_Button1(true);
                    break;

                default:
                    break;
            }
        }

        function onKeyUp(event) {
            var key = event.keyCode || event.which;
            switch (key) {
                case 48: //"0"
                    galaga.set_InsertCoin(false);
                    break;

                case 49: //"1"
                    galaga.set_Start1Player(false);
                    break;

                case 50: //"2"
                    galaga.set_Start2Player(false);
                    break;

                case 37: //"Left"
                    galaga.set_MoveLeft(false);
                    break;

                case 39: //"Right"
                    galaga.set_MoveRight(false);
                    break;

                case 17: //"Control"
                    galaga.set_Button1(false);
                    break;

                default:
                    break;
            }
        }

        (function (i, s, o, g, r, a, m) {
            i['GoogleAnalyticsObject'] = r;
            i[r] = i[r] || function () {
                    (i[r].q = i[r].q || []).push(arguments)
                }, i[r].l = 1 * new Date();
            a = s.createElement(o),
                m = s.getElementsByTagName(o)[0];
            a.async = 1;
            a.src = g;
            m.parentNode.insertBefore(a, m)
        })(window, document, 'script', '//www.google-analytics.com/analytics.js', 'ga');

        ga('create', 'UA-29328129-2', 'auto');
        ga('send', 'pageview');