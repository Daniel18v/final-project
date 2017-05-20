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
 * A class extending JSSMS.Z80 to debug the internal cpu logic.
 *
 * @constructor
 * @extends {JSSMS.Z80}
 */
JSSMS.Debugger = function() {
  // Nothing here -- never executed.
};

JSSMS.Debugger.prototype = {
  // Hold the ROM instructions parsed.
  instructions: [],


  /**
   * Reset the properties to their default values.
   * Called whenever a rom is loaded.
   */
  resetDebug: function() {
    this.instructions = [];

    this.main.ui.updateStatus('Parsing instructions...');
    this.parseInstructions();
    this.main.ui.updateStatus('Instructions parsed');
  },


  /**
   * Parse the rom instructions.
   */
  parseInstructions: function() {
    JSSMS.Utils.console.time('Instructions parsing');

    var romSize = PAGE_SIZE * this.rom.length;
    var instruction;
    var currentAddress;
    var i = 0;
    var addresses = []; //Array(romSize);

    var entryPoints = [
      0x00, // Add program entry point to the list of addresses to visit.
      0x38, // RST 38h and Interrupt.
      0x66 // Set by NMI.
    ];

    entryPoints.forEach(function(entryPoint) {
      addresses.push(entryPoint);
    });

    while (addresses.length) {
      currentAddress = addresses.shift();

      if (this.instructions[currentAddress]) {
        continue;
      }

      if (currentAddress >= romSize || (currentAddress >> 10) >= 65) {
        JSSMS.Utils.console.log('Invalid address', JSSMS.Utils.toHex(currentAddress));

        continue;
      }

      // @todo Move to a separate function to allow adding code entry points later.
      instruction = this.disassemble(currentAddress);
      this.instructions[currentAddress] = instruction;

      if (instruction.nextAddress !== null) {
        addresses.push(instruction.nextAddress);
      }

      if (instruction.target !== null) {
        addresses.push(instruction.target);
      }
    }

    // Flag any instructions that are jump targets.
    entryPoints.forEach(function(entryPoint) {
      if (!this.instructions[entryPoint]) {
        return;
      }
      this.instructions[entryPoint].isJumpTarget = true;
    }, this);

    for (; i < romSize; i++) {
      if (!this.instructions[i]) {
        continue;
      }
      // Comparing with null is important here as `0` is a valid address.
      if (this.instructions[i].nextAddress !== null && this.instructions[this.instructions[i].nextAddress]) {
        this.instructions[this.instructions[i].nextAddress].jumpTargetNb++;
      }
      if (this.instructions[i].target !== null) {
        if (this.instructions[this.instructions[i].target]) {
          this.instructions[this.instructions[i].target].isJumpTarget = true;
          this.instructions[this.instructions[i].target].jumpTargetNb++;
        } else {
          JSSMS.Utils.console.log('Invalid target address', JSSMS.Utils.toHex(this.instructions[i].target));
        }
      }
    }

    JSSMS.Utils.console.timeEnd('Instructions parsing');
  },


  /**
   * Write a dot file representation of parsed instructions to the console.
   *
   * @return {string} The content of a Dot file.
   */
  writeGraphViz: function() {
    JSSMS.Utils.console.time('DOT generation');

    var tree = this.instructions;
    var INDENT = ' ';

    var content = ['digraph G {'];

    for (var i = 0, length = tree.length; i < length; i++) {
      if (!tree[i]) {
        continue;
      }

      content.push(INDENT + i + ' [label="' + tree[i].label + '"];');

      if (tree[i].target !== null) {
        content.push(INDENT + i + ' -> ' + tree[i].target + ';');
      }

      if (tree[i].nextAddress !== null) {
        content.push(INDENT + i + ' -> ' + tree[i].nextAddress + ';');
      }
    }

    content.push('}');
    content = content.join('\n');

    // Inject entry point styling.
    content = content.replace(/ 0 \[label="/, ' 0 [style=filled,color="#CC0000",label="');

    JSSMS.Utils.console.timeEnd('DOT generation');

    return content;
  },


  /**
   * Return a string representing a JavaScript code for the ROM instructions.
   * The format is a big switch unrolling the value of this.pc.
   * The idea is to avoid using this.getUint8() and this.getUint16().
   *
   * @return {string} JavaScript code.
   */
  writeJavaScript: function() {
    JSSMS.Utils.console.time('JavaScript generation');

    var tree = this.instructions;
    var toHex = JSSMS.Utils.toHex;
    var tstates = 0;
    var prevAddress = 0;
    var prevNextAddress = 0;
    var breakNeeded = false;

    // Memory pages
    var pageBreakPoint = 0x0400;
    var pageNumber = 0;

    var i = 0, length = 0;

    var code = [
      '"": {',
      '"": function() {',
      'throw "Bad address: " + JSSMS.Utils.toHex(this.pc);'
    ];

    for (i = 0, length = tree.length; i < length; i++) {
      if (!tree[i]) {
        continue;
      }

      // Break branches into several pages.
      if (prevAddress <= pageBreakPoint && tree[i].address >= pageBreakPoint) {
        code.push('this.pc = ' + toHex(tree[i].address) + ';');
        code.push('}');
        code.push('},');

        code.push('' + pageNumber + ': {');
        code.push('"": function() {');
        code.push('throw "Bad address: " + JSSMS.Utils.toHex(this.pc);');
        breakNeeded = true;

        // Set next memory break point.
        pageNumber++;
        pageBreakPoint = pageNumber * PAGE_SIZE;
      }

      if (tree[i].isJumpTarget || prevNextAddress !== tree[i].address || breakNeeded) {
        insertTStates();
        if (prevNextAddress && !breakNeeded) {
          code.push('this.pc = ' + toHex(prevNextAddress) + ';');
        }
        code.push('},');
        // `temp` is only used for variable declaration, not actually passed parameter.
        code.push('' + toHex(tree[i].address) + ': function(temp, location) {');
        //code.push('// Nb of instructions jumping here: ' + tree[i].jumpTargetNb);
      }

      //code.push('if (this.tstates <= 0) {this.pc = ' + toHex(tree[i].address) + '; if (this.eol()) return;}');

      // Comment for debugging.
      code.push('// ' + tree[i].label);

      breakNeeded = tree[i].code.substr(-7) === 'return;';

      // Decrement tstates.
      tstates += getTotalTStates(tree[i].opcodes);

      /*if (tstates >= this.main.cyclesPerLine) {
       insertTStates();
       code.push('if (this.tstates <= 0) {this.pc = ' + toHex(prevNextAddress) + '; return;}');
       }*/

      if (/return;/.test(tree[i].code) || /this\.tstates/.test(tree[i].code)) {
        insertTStates();
      }

      // Instruction.
      if (tree[i].code !== '') {
        code.push(tree[i].code);
      }

      prevAddress = tree[i].address;
      prevNextAddress = tree[i].nextAddress;
    }

    code.push('}'); // End of last branch
    code.push('}'); // End of branches object (the comma is inserted in the template).
    code = code.join('\n');

    JSSMS.Utils.console.timeEnd('JavaScript generation');

    return code;

    function getTotalTStates(opcodes) {
      var tstates = 0;

      switch (opcodes[0]) {
        case 0xCB:
          tstates = OP_CB_STATES[opcodes[1]];
          break;
        case 0xDD:
        case 0xFD:
          if (opcodes.length === 2) {
            tstates = OP_DD_STATES[opcodes[1]];
          } else {
            tstates = OP_INDEX_CB_STATES[opcodes[2]];
          }
          break;
        case 0xED:
          tstates = OP_ED_STATES[opcodes[1]];
          break;
        default:
          tstates = OP_STATES[opcodes[0]];
          break;
      }

      return tstates;
    }

    function insertTStates() {
      if (tstates) {
        code.push('this.tstates -= ' + toHex(tstates) + ';');
      }

      tstates = 0;
    }
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * @param {number} address
   * @return {Object}
   */
  disassemble: function(address) {
    var toHex = JSSMS.Utils.toHex;
    var opcode = this.readRom8bit(address);
    var opcodesArray = [opcode];
    var inst = 'Unknown Opcode';
    var currAddr = address;
    var target = null;
    var code = 'throw "Unimplemented opcode ' + toHex(opcode) + '";';
    var operand = '';
    var location = 0;
    address++;

    var _inst = {};

    switch (opcode) {
      case 0x00:
        inst = 'NOP';
        code = '';
        break;
      case 0x01:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD BC,' + operand;
        code = 'this.setBC(' + operand + ');';
        address += 2;
        break;
      case 0x02:
        inst = 'LD (BC),A';
        code = 'this.setUint8(this.getBC(), this.a);';
        break;
      case 0x03:
        inst = 'INC BC';
        code = 'this.incBC();';
        break;
      case 0x04:
        inst = 'INC B';
        code = 'this.b = this.inc8(this.b);';
        break;
      case 0x05:
        inst = 'DEC B';
        code = 'this.b = this.dec8(this.b);';
        break;
      case 0x06:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD B,' + operand;
        code = 'this.b = ' + operand + ';';
        address++;
        break;
      case 0x07:
        inst = 'RLCA';
        code = 'this.rlca_a();';
        break;
      case 0x08:
        inst = 'EX AF AF\'';
        code = 'this.exAF();';
        break;
      case 0x09:
        inst = 'ADD HL,BC';
        code = 'this.setHL(this.add16(this.getHL(), this.getBC()));';
        break;
      case 0x0A:
        inst = 'LD A,(BC)';
        code = 'this.a = this.getUint8(this.getBC());';
        break;
      case 0x0B:
        inst = 'DEC BC';
        code = 'this.decBC();';
        break;
      case 0x0C:
        inst = 'INC C';
        code = 'this.c = this.inc8(this.c);';
        break;
      case 0x0D:
        inst = 'DEC C';
        code = 'this.c = this.dec8(this.c);';
        break;
      case 0x0E:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD C,' + operand;
        code = 'this.c = ' + operand + ';';
        address++;
        break;
      case 0x0F:
        inst = 'RRCA';
        code = 'this.rrca_a();';
        break;
      case 0x10:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'DJNZ (' + toHex(target) + ')';
        code = 'this.b = this.b - 0x01 & 0xFF;' +
            'if (this.b !== 0x00) {' +
            'this.tstates -= 0x05;' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address++;
        break;
      case 0x11:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD DE,' + operand;
        code = 'this.setDE(' + operand + ');';
        address += 2;
        break;
      case 0x12:
        inst = 'LD (DE),A';
        code = 'this.setUint8(this.getDE(), this.a);';
        break;
      case 0x13:
        inst = 'INC DE';
        code = 'this.incDE();';
        break;
      case 0x14:
        inst = 'INC D';
        code = 'this.d = this.inc8(this.d);';
        break;
      case 0x15:
        inst = 'DEC D';
        code = 'this.d = this.dec8(this.d);';
        break;
      case 0x16:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD D,' + operand;
        code = 'this.d = ' + operand + ';';
        address++;
        break;
      case 0x17:
        inst = 'RLA';
        code = 'this.rla_a();';
        break;
      case 0x18:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'JR (' + toHex(target) + ')';
        code = 'this.pc = ' + toHex(target) + '; return;';
        address = null;
        break;
      case 0x19:
        inst = 'ADD HL,DE';
        code = 'this.setHL(this.add16(this.getHL(), this.getDE()));';
        break;
      case 0x1A:
        inst = 'LD A,(DE)';
        code = 'this.a = this.getUint8(this.getDE());';
        break;
      case 0x1B:
        inst = 'DEC DE';
        code = 'this.decDE();';
        break;
      case 0x1C:
        inst = 'INC E';
        code = 'this.e = this.inc8(this.e);';
        break;
      case 0x1D:
        inst = 'DEC E';
        code = 'this.e = this.dec8(this.e);';
        break;
      case 0x1E:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD E,' + operand;
        code = 'this.e = ' + operand + ';';
        address++;
        break;
      case 0x1F:
        inst = 'RRA';
        code = 'this.rra_a();';
        break;
      case 0x20:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'JR NZ,(' + toHex(target) + ')';
        code = 'if (!((this.f & F_ZERO) !== 0x00)) {' +
            'this.tstates -= 0x05;' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address++;
        break;
      case 0x21:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD HL,' + operand;
        code = 'this.setHL(' + operand + ');';
        address += 2;
        break;
      case 0x22:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),HL';
        code = 'this.setUint8(' + operand + ', this.l);' +
            'this.setUint8(' + toHex(location + 1) + ', this.h);';
        address += 2;
        break;
      case 0x23:
        inst = 'INC HL';
        code = 'this.incHL();';
        break;
      case 0x24:
        inst = 'INC H';
        code = 'this.h = this.inc8(this.h);';
        break;
      case 0x25:
        inst = 'DEC H';
        code = 'this.h = this.dec8(this.h);';
        break;
      case 0x26:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD H,' + operand;
        code = 'this.h = ' + operand + ';';
        address++;
        break;
      case 0x27:
        inst = 'DAA';
        code = 'this.daa();';
        break;
      case 0x28:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'JR Z,(' + toHex(target) + ')';
        code = 'if ((this.f & F_ZERO) !== 0x00) {' +
            'this.tstates -= 0x05;' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address++;
        break;
      case 0x29:
        inst = 'ADD HL,HL';
        code = 'this.setHL(this.add16(this.getHL(), this.getHL()));';
        break;
      case 0x2A:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD HL,(' + operand + ')';
        code = 'this.setHL(this.getUint16(' + operand + '));';
        address += 2;
        break;
      case 0x2B:
        inst = 'DEC HL';
        code = 'this.decHL();';
        break;
      case 0x2C:
        inst = 'INC L';
        code = 'this.l = this.inc8(this.l);';
        break;
      case 0x2D:
        inst = 'DEC L';
        code = 'this.l = this.dec8(this.l);';
        break;
      case 0x2E:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD L,' + operand;
        code = 'this.l = ' + operand + ';';
        address++;
        break;
      case 0x2F:
        inst = 'CPL';
        code = 'this.cpl_a();';
        break;
      case 0x30:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'JR NC,(' + toHex(target) + ')';
        code = 'if (!((this.f & F_CARRY) !== 0x00)) {' +
            'this.tstates -= 0x05;' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address++;
        break;
      case 0x31:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD SP,' + operand;
        code = 'this.sp = ' + operand + ';';
        address += 2;
        break;
      case 0x32:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD (' + operand + '),A';
        code = 'this.setUint8(' + operand + ', this.a);';
        address += 2;
        break;
      case 0x33:
        inst = 'INC SP';
        code = 'this.sp++;';
        break;
      case 0x34:
        inst = 'INC (HL)';
        code = 'this.incMem(this.getHL());';
        break;
      case 0x35:
        inst = 'DEC (HL)';
        code = 'this.decMem(this.getHL());';
        break;
      case 0x36:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD (HL),' + operand;
        code = 'this.setUint8(this.getHL(), ' + operand + ');';
        address++;
        break;
      case 0x37:
        inst = 'SCF';
        code = 'this.f |= F_CARRY; this.f &= ~ F_NEGATIVE; this.f &= ~ F_HALFCARRY;';
        break;
      case 0x38:
        target = address + this.signExtend(this.readRom8bit(address) + 1);
        inst = 'JR C,(' + toHex(target) + ')';
        code = 'if ((this.f & F_CARRY) !== 0x00) {' +
            'this.tstates -= 0x05;' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address++;
        break;
      case 0x39:
        inst = 'ADD HL,SP';
        code = 'this.setHL(this.add16(this.getHL(), this.sp));';
        break;
      case 0x3A:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD A,(' + operand + ')';
        code = 'this.a = this.getUint8(' + operand + ');';
        address += 2;
        break;
      case 0x3B:
        inst = 'DEC SP';
        code = 'this.sp--;';
        break;
      case 0x3C:
        inst = 'INC A';
        code = 'this.a = this.inc8(this.a);';
        break;
      case 0x3D:
        inst = 'DEC A';
        code = 'this.a = this.dec8(this.a);';
        break;
      case 0x3E:
        operand = toHex(this.readRom8bit(address));
        inst = 'LD A,' + operand;
        code = 'this.a = ' + operand + ';';
        address++;
        break;
      case 0x3F:
        inst = 'CCF';
        code = 'this.ccf();';
        break;
      case 0x40:
        inst = 'LD B,B';
        code = '';
        break;
      case 0x41:
        inst = 'LD B,C';
        code = 'this.b = this.c;';
        break;
      case 0x42:
        inst = 'LD B,D';
        code = 'this.b = this.d;';
        break;
      case 0x43:
        inst = 'LD B,E';
        code = 'this.b = this.e;';
        break;
      case 0x44:
        inst = 'LD B,H';
        code = 'this.b = this.h;';
        break;
      case 0x45:
        inst = 'LD B,L';
        code = 'this.b = this.l;';
        break;
      case 0x46:
        inst = 'LD B,(HL)';
        code = 'this.b = this.getUint8(this.getHL());';
        break;
      case 0x47:
        inst = 'LD B,A';
        code = 'this.b = this.a;';
        break;
      case 0x48:
        inst = 'LD C,B';
        code = 'this.c = this.b;';
        break;
      case 0x49:
        inst = 'LD C,C';
        code = '';
        break;
      case 0x4A:
        inst = 'LD C,D';
        code = 'this.c = this.d;';
        break;
      case 0x4B:
        inst = 'LD C,E';
        code = 'this.c = this.e;';
        break;
      case 0x4C:
        inst = 'LD C,H';
        code = 'this.c = this.h;';
        break;
      case 0x4D:
        inst = 'LD C,L';
        code = 'this.c = this.l;';
        break;
      case 0x4E:
        inst = 'LD C,(HL)';
        code = 'this.c = this.getUint8(this.getHL());';
        break;
      case 0x4F:
        inst = 'LD C,A';
        code = 'this.c = this.a;';
        break;
      case 0x50:
        inst = 'LD D,B';
        code = 'this.d = this.b;';
        break;
      case 0x51:
        inst = 'LD D,C';
        code = 'this.d = this.c;';
        break;
      case 0x52:
        inst = 'LD D,D';
        code = '';
        break;
      case 0x53:
        inst = 'LD D,E';
        code = 'this.d = this.e;';
        break;
      case 0x54:
        inst = 'LD D,H';
        code = 'this.d = this.h;';
        break;
      case 0x55:
        inst = 'LD D,L';
        code = 'this.d = this.l;';
        break;
      case 0x56:
        inst = 'LD D,(HL)';
        code = 'this.d = this.getUint8(this.getHL());';
        break;
      case 0x57:
        inst = 'LD D,A';
        code = 'this.d = this.a;';
        break;
      case 0x58:
        inst = 'LD E,B';
        code = 'this.e = this.b;';
        break;
      case 0x59:
        inst = 'LD E,C';
        code = 'this.e = this.c;';
        break;
      case 0x5A:
        inst = 'LD E,D';
        code = 'this.e = this.d;';
        break;
      case 0x5B:
        inst = 'LD E,E';
        code = '';
        break;
      case 0x5C:
        inst = 'LD E,H';
        code = 'this.e = this.h;';
        break;
      case 0x5D:
        inst = 'LD E,L';
        code = 'this.e = this.l;';
        break;
      case 0x5E:
        inst = 'LD E,(HL)';
        code = 'this.e = this.getUint8(this.getHL());';
        break;
      case 0x5F:
        inst = 'LD E,A';
        code = 'this.e = this.a;';
        break;
      case 0x60:
        inst = 'LD H,B';
        code = 'this.h = this.b;';
        break;
      case 0x61:
        inst = 'LD H,C';
        code = 'this.h = this.c;';
        break;
      case 0x62:
        inst = 'LD H,D';
        code = 'this.h = this.d;';
        break;
      case 0x63:
        inst = 'LD H,E';
        code = 'this.h = this.e;';
        break;
      case 0x64:
        inst = 'LD H,H';
        code = '';
        break;
      case 0x65:
        inst = 'LD H,L';
        code = 'this.h = this.l;';
        break;
      case 0x66:
        inst = 'LD H,(HL)';
        code = 'this.h = this.getUint8(this.getHL());';
        break;
      case 0x67:
        inst = 'LD H,A';
        code = 'this.h = this.a;';
        break;
      case 0x68:
        inst = 'LD L,B';
        code = 'this.l = this.b;';
        break;
      case 0x69:
        inst = 'LD L,C';
        code = 'this.l = this.c;';
        break;
      case 0x6A:
        inst = 'LD L,D';
        code = 'this.l = this.d;';
        break;
      case 0x6B:
        inst = 'LD L,E';
        code = 'this.l = this.e;';
        break;
      case 0x6C:
        inst = 'LD L,H';
        code = 'this.l = this.h;';
        break;
      case 0x6D:
        inst = 'LD L,L';
        code = '';
        break;
      case 0x6E:
        inst = 'LD L,(HL)';
        code = 'this.l = this.getUint8(this.getHL());';
        break;
      case 0x6F:
        inst = 'LD L,A';
        code = 'this.l = this.a;';
        break;
      case 0x70:
        inst = 'LD (HL),B';
        code = 'this.setUint8(this.getHL(), this.b);';
        break;
      case 0x71:
        inst = 'LD (HL),C';
        code = 'this.setUint8(this.getHL(), this.c);';
        break;
      case 0x72:
        inst = 'LD (HL),D';
        code = 'this.setUint8(this.getHL(), this.d);';
        break;
      case 0x73:
        inst = 'LD (HL),E';
        code = 'this.setUint8(this.getHL(), this.e);';
        break;
      case 0x74:
        inst = 'LD (HL),H';
        code = 'this.setUint8(this.getHL(), this.h);';
        break;
      case 0x75:
        inst = 'LD (HL),L';
        code = 'this.setUint8(this.getHL(), this.l);';
        break;
      case 0x76:
        inst = 'HALT';
        if (HALT_SPEEDUP) {
          code = 'this.tstates = 0x00;';
        } else {
          code = '';
        }
        code += 'this.halt = true; this.pc = ' + toHex(address - 1) + '; return;';
        break;
      case 0x77:
        inst = 'LD (HL),A';
        code = 'this.setUint8(this.getHL(), this.a);';
        break;
      case 0x78:
        inst = 'LD A,B';
        code = 'this.a = this.b;';
        break;
      case 0x79:
        inst = 'LD A,C';
        code = 'this.a = this.c;';
        break;
      case 0x7A:
        inst = 'LD A,D';
        code = 'this.a = this.d;';
        break;
      case 0x7B:
        inst = 'LD A,E';
        code = 'this.a = this.e;';
        break;
      case 0x7C:
        inst = 'LD A,H';
        code = 'this.a = this.h;';
        break;
      case 0x7D:
        inst = 'LD A,L';
        code = 'this.a = this.l;';
        break;
      case 0x7E:
        inst = 'LD A,(HL)';
        code = 'this.a = this.getUint8(this.getHL());';
        break;
      case 0x7F:
        inst = 'LD A,A';
        code = '';
        break;
      case 0x80:
        inst = 'ADD A,B';
        code = 'this.add_a(this.b);';
        break;
      case 0x81:
        inst = 'ADD A,C';
        code = 'this.add_a(this.c);';
        break;
      case 0x82:
        inst = 'ADD A,D';
        code = 'this.add_a(this.d);';
        break;
      case 0x83:
        inst = 'ADD A,E';
        code = 'this.add_a(this.e);';
        break;
      case 0x84:
        inst = 'ADD A,H';
        code = 'this.add_a(this.h);';
        break;
      case 0x85:
        inst = 'ADD A,L';
        code = 'this.add_a(this.l);';
        break;
      case 0x86:
        inst = 'ADD A,(HL)';
        code = 'this.add_a(this.getUint8(this.getHL()));';
        break;
      case 0x87:
        inst = 'ADD A,A';
        code = 'this.add_a(this.a);';
        break;
      case 0x88:
        inst = 'ADC A,B';
        code = 'this.adc_a(this.b);';
        break;
      case 0x89:
        inst = 'ADC A,C';
        code = 'this.adc_a(this.c);';
        break;
      case 0x8A:
        inst = 'ADC A,D';
        code = 'this.adc_a(this.d);';
        break;
      case 0x8B:
        inst = 'ADC A,E';
        code = 'this.adc_a(this.e);';
        break;
      case 0x8C:
        inst = 'ADC A,H';
        code = 'this.adc_a(this.h);';
        break;
      case 0x8D:
        inst = 'ADC A,L';
        code = 'this.adc_a(this.l);';
        break;
      case 0x8E:
        inst = 'ADC A,(HL)';
        code = 'this.adc_a(this.getUint8(this.getHL()));';
        break;
      case 0x8F:
        inst = 'ADC A,A';
        code = 'this.adc_a(this.a);';
        break;
      case 0x90:
        inst = 'SUB A,B';
        code = 'this.sub_a(this.b);';
        break;
      case 0x91:
        inst = 'SUB A,C';
        code = 'this.sub_a(this.c);';
        break;
      case 0x92:
        inst = 'SUB A,D';
        code = 'this.sub_a(this.d);';
        break;
      case 0x93:
        inst = 'SUB A,E';
        code = 'this.sub_a(this.e);';
        break;
      case 0x94:
        inst = 'SUB A,H';
        code = 'this.sub_a(this.h);';
        break;
      case 0x95:
        inst = 'SUB A,L';
        code = 'this.sub_a(this.l);';
        break;
      case 0x96:
        inst = 'SUB A,(HL)';
        code = 'this.sub_a(this.getUint8(this.getHL()));';
        break;
      case 0x97:
        inst = 'SUB A,A';
        code = 'this.sub_a(this.a);';
        break;
      case 0x98:
        inst = 'SBC A,B';
        code = 'this.sbc_a(this.b);';
        break;
      case 0x99:
        inst = 'SBC A,C';
        code = 'this.sbc_a(this.c);';
        break;
      case 0x9A:
        inst = 'SBC A,D';
        code = 'this.sbc_a(this.d);';
        break;
      case 0x9B:
        inst = 'SBC A,E';
        code = 'this.sbc_a(this.e);';
        break;
      case 0x9C:
        inst = 'SBC A,H';
        code = 'this.sbc_a(this.h);';
        break;
      case 0x9D:
        inst = 'SBC A,L';
        code = 'this.sbc_a(this.l);';
        break;
      case 0x9E:
        inst = 'SBC A,(HL)';
        code = 'this.sbc_a(this.getUint8(this.getHL()));';
        break;
      case 0x9F:
        inst = 'SBC A,A';
        code = 'this.sbc_a(this.a);';
        break;
      case 0xA0:
        inst = 'AND A,B';
        code = 'this.f = this.SZP_TABLE[this.a &= this.b] | F_HALFCARRY;';
        break;
      case 0xA1:
        inst = 'AND A,C';
        code = 'this.f = this.SZP_TABLE[this.a &= this.c] | F_HALFCARRY;';
        break;
      case 0xA2:
        inst = 'AND A,D';
        code = 'this.f = this.SZP_TABLE[this.a &= this.d] | F_HALFCARRY;';
        break;
      case 0xA3:
        inst = 'AND A,E';
        code = 'this.f = this.SZP_TABLE[this.a &= this.e] | F_HALFCARRY;';
        break;
      case 0xA4:
        inst = 'AND A,H';
        code = 'this.f = this.SZP_TABLE[this.a &= this.h] | F_HALFCARRY;';
        break;
      case 0xA5:
        inst = 'AND A,L';
        code = 'this.f = this.SZP_TABLE[this.a &= this.l] | F_HALFCARRY;';
        break;
      case 0xA6:
        inst = 'AND A,(HL)';
        code = 'this.f = this.SZP_TABLE[this.a &= this.getUint8(this.getHL())] | F_HALFCARRY;';
        break;
      case 0xA7:
        inst = 'AND A,A';
        code = 'this.f = this.SZP_TABLE[this.a] | F_HALFCARRY;';
        break;
      case 0xA8:
        inst = 'XOR A,B';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.b];';
        break;
      case 0xA9:
        inst = 'XOR A,C';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.c];';
        break;
      case 0xAA:
        inst = 'XOR A,D';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.d];';
        break;
      case 0xAB:
        inst = 'XOR A,E';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.e];';
        break;
      case 0xAC:
        inst = 'XOR A,H';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.h];';
        break;
      case 0xAD:
        inst = 'XOR A,L';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.l];';
        break;
      case 0xAE:
        inst = 'XOR A,(HL)';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.getUint8(this.getHL())];';
        break;
      case 0xAF:
        inst = 'XOR A,A'; // =0
        //code = 'this.a = ' + toHex(0) + '; this.f = ' + toHex(this.SZP_TABLE[0]) + ';';
        code = 'this.a = ' + toHex(0) + '; this.f = this.SZP_TABLE[0x00];';
        break;
      case 0xB0:
        inst = 'OR A,B';
        code = 'this.f = this.SZP_TABLE[this.a |= this.b];';
        break;
      case 0xB1:
        inst = 'OR A,C';
        code = 'this.f = this.SZP_TABLE[this.a |= this.c];';
        break;
      case 0xB2:
        inst = 'OR A,D';
        code = 'this.f = this.SZP_TABLE[this.a |= this.d];';
        break;
      case 0xB3:
        inst = 'OR A,E';
        code = 'this.f = this.SZP_TABLE[this.a |= this.e];';
        break;
      case 0xB4:
        inst = 'OR A,H';
        code = 'this.f = this.SZP_TABLE[this.a |= this.h];';
        break;
      case 0xB5:
        inst = 'OR A,L';
        code = 'this.f = this.SZP_TABLE[this.a |= this.l];';
        break;
      case 0xB6:
        inst = 'OR A,(HL)';
        code = 'this.f = this.SZP_TABLE[this.a |= this.getUint8(this.getHL())];';
        break;
      case 0xB7:
        inst = 'OR A,A';
        code = 'this.f = this.SZP_TABLE[this.a];';
        break;
      case 0xB8:
        inst = 'CP A,B';
        code = 'this.cp_a(this.b);';
        break;
      case 0xB9:
        inst = 'CP A,C';
        code = 'this.cp_a(this.c);';
        break;
      case 0xBA:
        inst = 'CP A,D';
        code = 'this.cp_a(this.d);';
        break;
      case 0xBB:
        inst = 'CP A,E';
        code = 'this.cp_a(this.e);';
        break;
      case 0xBC:
        inst = 'CP A,H';
        code = 'this.cp_a(this.h);';
        break;
      case 0xBD:
        inst = 'CP A,L';
        code = 'this.cp_a(this.l);';
        break;
      case 0xBE:
        inst = 'CP A,(HL)';
        code = 'this.cp_a(this.getUint8(this.getHL()));';
        break;
      case 0xBF:
        inst = 'CP A,A';
        code = 'this.cp_a(this.a);';
        break;
      case 0xC0:
        inst = 'RET NZ';
        code = 'if ((this.f & F_ZERO) === 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xC1:
        inst = 'POP BC';
        code = 'this.setBC(this.getUint16(this.sp)); this.sp += 0x02;';
        break;
      case 0xC2:
        target = this.readRom16bit(address);
        inst = 'JP NZ,(' + toHex(target) + ')';
        code = 'if ((this.f & F_ZERO) === 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xC3:
        target = this.readRom16bit(address);
        inst = 'JP (' + toHex(target) + ')';
        code = 'this.pc = ' + toHex(target) + '; return;';
        address = null;
        break;
      case 0xC4:
        target = this.readRom16bit(address);
        inst = 'CALL NZ (' + toHex(target) + ')';
        code = 'if ((this.f & F_ZERO) === 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xC5:
        inst = 'PUSH BC';
        code = 'this.push2(this.b, this.c);';
        break;
      case 0xC6:
        operand = toHex(this.readRom8bit(address));
        inst = 'ADD A,' + operand;
        code = 'this.add_a(' + operand + ');';
        address++;
        break;
      case 0xC7:
        target = 0x00;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xC8:
        inst = 'RET Z';
        code = 'if ((this.f & F_ZERO) !== 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xC9:
        inst = 'RET';
        code = 'this.pc = this.getUint16(this.sp); this.sp += 0x02; return;';
        address = null;
        break;
      case 0xCA:
        target = this.readRom16bit(address);
        inst = 'JP Z,(' + toHex(target) + ')';
        code = 'if ((this.f & F_ZERO) !== 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xCB:
        _inst = this.getCB(address);
        inst = _inst.inst;
        code = _inst.code;
        opcodesArray = opcodesArray.concat(_inst.opcodes);
        address = _inst.nextAddress;
        // @todo
        break;
      case 0xCC:
        target = this.readRom16bit(address);
        inst = 'CALL Z (' + toHex(target) + ')';
        code = 'if ((this.f & F_ZERO) !== 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xCD:
        target = this.readRom16bit(address);
        inst = 'CALL (' + toHex(target) + ')';
        code = 'this.push1(' + toHex(address + 2) + '); this.pc = ' + toHex(target) + '; return;';
        address += 2;
        break;
      case 0xCE:
        operand = toHex(this.readRom8bit(address));
        inst = 'ADC ,' + operand;
        code = 'this.adc_a(' + operand + ');';
        address++;
        break;
      case 0xCF:
        target = 0x08;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xD0:
        inst = 'RET NC';
        code = 'if ((this.f & F_CARRY) === 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xD1:
        inst = 'POP DE';
        code = 'this.setDE(this.getUint16(this.sp)); this.sp += 0x02;';
        break;
      case 0xD2:
        target = this.readRom16bit(address);
        inst = 'JP NC,(' + toHex(target) + ')';
        code = 'if ((this.f & F_CARRY) === 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xD3:
        operand = this.readRom8bit(address);
        inst = 'OUT (' + toHex(operand) + '),A';
        code = this.peepholePortOut(operand);
        address++;
        break;
      case 0xD4:
        target = this.readRom16bit(address);
        inst = 'CALL NC (' + toHex(target) + ')';
        code = 'if ((this.f & F_CARRY) === 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xD5:
        inst = 'PUSH DE';
        code = 'this.push2(this.d, this.e);';
        break;
      case 0xD6:
        operand = toHex(this.readRom8bit(address));
        inst = 'SUB ' + operand;
        code = 'this.sub_a(' + operand + ');';
        address++;
        break;
      case 0xD7:
        target = 0x10;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xD8:
        inst = 'RET C';
        code = 'if ((this.f & F_CARRY) !== 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xD9:
        inst = 'EXX';
        // @todo Expand these functions?
        code = 'this.exBC(); this.exDE(); this.exHL();';
        break;
      case 0xDA:
        target = this.readRom16bit(address);
        inst = 'JP C,(' + toHex(target) + ')';
        code = 'if ((this.f & F_CARRY) !== 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xDB:
        operand = this.readRom8bit(address);
        inst = 'IN A,(' + toHex(operand) + ')';
        code = this.peepholePortIn(operand);
        address++;
        break;
      case 0xDC:
        target = this.readRom16bit(address);
        inst = 'CALL C (' + toHex(target) + ')';
        code = 'if ((this.f & F_CARRY) !== 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xDD:
        _inst = this.getIndexOpIX(address);
        inst = _inst.inst;
        code = _inst.code;
        opcodesArray = opcodesArray.concat(_inst.opcodes);
        address = _inst.nextAddress;
        // @todo
        break;
      case 0xDE:
        operand = toHex(this.readRom8bit(address));
        inst = 'SBC A,' + operand;
        code = 'this.sbc_a(' + operand + ');';
        address++;
        break;
      case 0xDF:
        target = 0x18;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xE0:
        inst = 'RET PO';
        code = 'if ((this.f & F_PARITY) === 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xE1:
        inst = 'POP HL';
        code = 'this.setHL(this.getUint16(this.sp)); this.sp += 0x02;';
        break;
      case 0xE2:
        target = this.readRom16bit(address);
        inst = 'JP PO,(' + toHex(target) + ')';
        code = 'if ((this.f & F_PARITY) === 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xE3:
        inst = 'EX (SP),HL';
        code = 'temp = this.h;' +
            'this.h = this.getUint8(this.sp + 0x01);' +
            'this.setUint8(this.sp + 0x01, temp);' +
            'temp = this.l;' +
            'this.l = this.getUint8(this.sp);' +
            'this.setUint8(this.sp, temp);';
        break;
      case 0xE4:
        target = this.readRom16bit(address);
        inst = 'CALL PO (' + toHex(target) + ')';
        code = 'if ((this.f & F_PARITY) === 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xE5:
        inst = 'PUSH HL';
        code = 'this.push2(this.h, this.l);';
        break;
      case 0xE6:
        operand = toHex(this.readRom8bit(address));
        inst = 'AND (' + operand + ')';
        code = 'this.f = this.SZP_TABLE[this.a &= ' + operand + '] | F_HALFCARRY;';
        address++;
        break;
      case 0xE7:
        target = 0x20;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xE8:
        inst = 'RET PE';
        code = 'if ((this.f & F_PARITY) !== 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xE9:
        // This target can't be determined using static analysis.
        inst = 'JP (HL)';
        code = 'this.pc = this.getHL(); return;';
        address = null;
        break;
      case 0xEA:
        target = this.readRom16bit(address);
        inst = 'JP PE,(' + toHex(target) + ')';
        code = 'if ((this.f & F_PARITY) !== 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xEB:
        inst = 'EX DE,HL';
        code = 'temp = this.d;' +
            'this.d = this.h;' +
            'this.h = temp;' +
            'temp = this.e;' +
            'this.e = this.l;' +
            'this.l = temp;';
        break;
      case 0xEC:
        target = this.readRom16bit(address);
        inst = 'CALL PE (' + toHex(target) + ')';
        code = 'if ((this.f & F_PARITY) !== 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xED:
        _inst = this.getED(address);
        target = _inst.target;
        inst = _inst.inst;
        code = _inst.code;
        opcodesArray = opcodesArray.concat(_inst.opcodes);
        address = _inst.nextAddress;
        // @todo
        break;
      case 0xEE:
        operand = toHex(this.readRom8bit(address));
        inst = 'XOR ' + operand;
        code = 'this.f = this.SZP_TABLE[this.a ^= ' + operand + '];';
        address++;
        break;
      case 0xEF:
        target = 0x28;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xF0:
        inst = 'RET P';
        code = 'if ((this.f & F_SIGN) === 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xF1:
        inst = 'POP AF';
        code = 'this.setAF(this.getUint16(this.sp)); this.sp += 0x02;';
        break;
      case 0xF2:
        target = this.readRom16bit(address);
        inst = 'JP P,(' + toHex(target) + ')';
        code = 'if ((this.f & F_SIGN) === 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xF3:
        inst = 'DI';
        code = 'this.iff1 = false; this.iff2 = false; this.EI_inst = true;';
        break;
      case 0xF4:
        target = this.readRom16bit(address);
        inst = 'CALL P (' + toHex(target) + ')';
        code = 'if ((this.f & F_SIGN) === 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xF5:
        inst = 'PUSH AF';
        code = 'this.push2(this.a, this.f);';
        break;
      case 0xF6:
        operand = toHex(this.readRom8bit(address));
        inst = 'OR ' + operand;
        code = 'this.f = this.SZP_TABLE[this.a |= ' + operand + '];';
        address++;
        break;
      case 0xF7:
        target = 0x30;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
      case 0xF8:
        inst = 'RET M';
        code = 'if ((this.f & F_SIGN) !== 0x00) {' +
            'this.tstates -= 0x06;' +
            'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'return;' +
            '}';
        break;
      case 0xF9:
        inst = 'LD SP,HL';
        code = 'this.sp = this.getHL()';
        break;
      case 0xFA:
        target = this.readRom16bit(address);
        inst = 'JP M,(' + toHex(target) + ')';
        code = 'if ((this.f & F_SIGN) !== 0x00) {' +
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xFB:
        inst = 'EI';
        code = 'this.iff1 = true; this.iff2 = true; this.EI_inst = true;';
        break;
      case 0xFC:
        target = this.readRom16bit(address);
        inst = 'CALL M (' + toHex(target) + ')';
        code = 'if ((this.f & F_SIGN) !== 0x00) {' +
            'this.tstates -= 0x07;' +
            'this.push1(' + toHex(address + 2) + ');' + // write value of PC to stack
            'this.pc = ' + toHex(target) + ';' +
            'return;' +
            '}';
        address += 2;
        break;
      case 0xFD:
        _inst = this.getIndexOpIY(address);
        inst = _inst.inst;
        code = _inst.code;
        opcodesArray = opcodesArray.concat(_inst.opcodes);
        address = _inst.nextAddress;
        // @todo
        break;
      case 0xFE:
        operand = toHex(this.readRom8bit(address));
        inst = 'CP ' + operand;
        code = 'this.cp_a(' + operand + ');';
        address++;
        break;
      case 0xFF:
        target = 0x38;
        inst = 'RST ' + toHex(target);
        code = 'this.push1(' + toHex(address) + '); this.pc = ' + toHex(target) + '; return;';
        break;
    }

    return Instruction({
      opcode: opcode,
      opcodes: opcodesArray,
      inst: inst,
      code: code,
      address: currAddr,
      nextAddress: address,
      target: target
    });
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * @param {number} address
   * @return {Object}
   */
  getCB: function(address) {
    var opcode = this.readRom8bit(address);
    var opcodesArray = [opcode];
    var inst = 'Unimplemented 0xCB prefixed opcode';
    var currAddr = address;
    var code = 'throw "Unimplemented 0xCB prefixed opcode";';
    address++;

    switch (opcode) {
      case 0x00:
        inst = 'RLC B';
        code = 'this.b = this.rlc(this.b);';
        break;
      case 0x01:
        inst = 'RLC C';
        code = 'this.c = this.rlc(this.c);';
        break;
      case 0x02:
        inst = 'RLC D';
        code = 'this.d = this.rlc(this.d);';
        break;
      case 0x03:
        inst = 'RLC E';
        code = 'this.e = this.rlc(this.e);';
        break;
      case 0x04:
        inst = 'RLC H';
        code = 'this.h = this.rlc(this.h);';
        break;
      case 0x05:
        inst = 'RLC L';
        code = 'this.l = this.rlc(this.l);';
        break;
      case 0x06:
        inst = 'RLC (HL)';
        code = 'this.setUint8(this.getHL(), this.rlc(this.getUint8(this.getHL())));';
        break;
      case 0x07:
        inst = 'RLC A';
        code = 'this.a = this.rlc(this.a);';
        break;
      case 0x08:
        inst = 'RRC B';
        code = 'this.b = this.rrc(this.b);';
        break;
      case 0x09:
        inst = 'RRC C';
        code = 'this.c = this.rrc(this.c);';
        break;
      case 0x0A:
        inst = 'RRC D';
        code = 'this.d = this.rrc(this.d);';
        break;
      case 0x0B:
        inst = 'RRC E';
        code = 'this.e = this.rrc(this.e);';
        break;
      case 0x0C:
        inst = 'RRC H';
        code = 'this.h = this.rrc(this.h);';
        break;
      case 0x0D:
        inst = 'RRC L';
        code = 'this.l = this.rrc(this.l);';
        break;
      case 0x0E:
        inst = 'RRC (HL)';
        code = 'this.setUint8(this.getHL(), this.rrc(this.getUint8(this.getHL())));';
        break;
      case 0x0F:
        inst = 'RRC A';
        code = 'this.a = this.rrc(this.a);';
        break;
      case 0x10:
        inst = 'RL B';
        code = 'this.b = this.rl(this.b);';
        break;
      case 0x11:
        inst = 'RL C';
        code = 'this.c = this.rl(this.c);';
        break;
      case 0x12:
        inst = 'RL D';
        code = 'this.d = this.rl(this.d);';
        break;
      case 0x13:
        inst = 'RL E';
        code = 'this.e = this.rl(this.e);';
        break;
      case 0x14:
        inst = 'RL H';
        code = 'this.h = this.rl(this.h);';
        break;
      case 0x15:
        inst = 'RL L';
        code = 'this.l = this.rl(this.l);';
        break;
      case 0x16:
        inst = 'RL (HL)';
        code = 'this.setUint8(this.getHL(), this.rl(this.getUint8(this.getHL())));';
        break;
      case 0x17:
        inst = 'RL A';
        code = 'this.a = this.rl(this.a);';
        break;
      case 0x18:
        inst = 'RR B';
        code = 'this.b = this.rr(this.b);';
        break;
      case 0x19:
        inst = 'RR C';
        code = 'this.c = this.rr(this.c);';
        break;
      case 0x1A:
        inst = 'RR D';
        code = 'this.d = this.rr(this.d);';
        break;
      case 0x1B:
        inst = 'RR E';
        code = 'this.e = this.rr(this.e);';
        break;
      case 0x1C:
        inst = 'RR H';
        code = 'this.h = this.rr(this.h);';
        break;
      case 0x1D:
        inst = 'RR L';
        code = 'this.l = this.rr(this.l);';
        break;
      case 0x1E:
        inst = 'RR (HL)';
        code = 'this.setUint8(this.getHL(), this.rr(this.getUint8(this.getHL())));';
        break;
      case 0x1F:
        inst = 'RR A';
        code = 'this.a = this.rr(this.a);';
        break;
      case 0x20:
        inst = 'SLA B';
        code = 'this.b = this.sla(this.b);';
        break;
      case 0x21:
        inst = 'SLA C';
        code = 'this.c = this.sla(this.c);';
        break;
      case 0x22:
        inst = 'SLA D';
        code = 'this.d = this.sla(this.d);';
        break;
      case 0x23:
        inst = 'SLA E';
        code = 'this.e = this.sla(this.e);';
        break;
      case 0x24:
        inst = 'SLA H';
        code = 'this.h = this.sla(this.h);';
        break;
      case 0x25:
        inst = 'SLA L';
        code = 'this.l = this.sla(this.l);';
        break;
      case 0x26:
        inst = 'SLA (HL)';
        code = 'this.setUint8(this.getHL(), this.sla(this.getUint8(this.getHL())));';
        break;
      case 0x27:
        inst = 'SLA A';
        code = 'this.a = this.sla(this.a);';
        break;
      case 0x28:
        inst = 'SRA B';
        code = 'this.b = this.sra(this.b);';
        break;
      case 0x29:
        inst = 'SRA C';
        code = 'this.c = this.sra(this.c);';
        break;
      case 0x2A:
        inst = 'SRA D';
        code = 'this.d = this.sra(this.d);';
        break;
      case 0x2B:
        inst = 'SRA E';
        code = 'this.e = this.sra(this.e);';
        break;
      case 0x2C:
        inst = 'SRA H';
        code = 'this.h = this.sra(this.h);';
        break;
      case 0x2D:
        inst = 'SRA L';
        code = 'this.l = this.sra(this.l);';
        break;
      case 0x2E:
        inst = 'SRA (HL)';
        code = 'this.setUint8(this.getHL(), this.sra(this.getUint8(this.getHL())));';
        break;
      case 0x2F:
        inst = 'SRA A';
        code = 'this.a = this.sra(this.a);';
        break;
      case 0x30:
        inst = 'SLL B';
        code = 'this.b = this.sll(this.b);';
        break;
      case 0x31:
        inst = 'SLL C';
        code = 'this.c = this.sll(this.c);';
        break;
      case 0x32:
        inst = 'SLL D';
        code = 'this.d = this.sll(this.d);';
        break;
      case 0x33:
        inst = 'SLL E';
        code = 'this.e = this.sll(this.e);';
        break;
      case 0x34:
        inst = 'SLL H';
        code = 'this.h = this.sll(this.h);';
        break;
      case 0x35:
        inst = 'SLL L';
        code = 'this.l = this.sll(this.l);';
        break;
      case 0x36:
        inst = 'SLL (HL)';
        code = 'this.setUint8(this.getHL(), this.sll(this.getUint8(this.getHL())));';
        break;
      case 0x37:
        inst = 'SLL A';
        code = 'this.a = this.sll(this.a);';
        break;
      case 0x38:
        inst = 'SRL B';
        code = 'this.b = this.srl(this.b);';
        break;
      case 0x39:
        inst = 'SRL C';
        code = 'this.c = this.srl(this.c);';
        break;
      case 0x3A:
        inst = 'SRL D';
        code = 'this.d = this.srl(this.d);';
        break;
      case 0x3B:
        inst = 'SRL E';
        code = 'this.e = this.srl(this.e);';
        break;
      case 0x3C:
        inst = 'SRL H';
        code = 'this.h = this.srl(this.h);';
        break;
      case 0x3D:
        inst = 'SRL L';
        code = 'this.l = this.srl(this.l);';
        break;
      case 0x3E:
        inst = 'SRL (HL)';
        code = 'this.setUint8(this.getHL(), this.srl(this.getUint8(this.getHL())));';
        break;
      case 0x3F:
        inst = 'SRL A';
        code = 'this.a = this.srl(this.a);';
        break;
      case 0x40:
        inst = 'BIT 0,B';
        code = 'this.bit(this.b & BIT_0);';
        break;
      case 0x41:
        inst = 'BIT 0,C';
        code = 'this.bit(this.c & BIT_0);';
        break;
      case 0x42:
        inst = 'BIT 0,D';
        code = 'this.bit(this.d & BIT_0);';
        break;
      case 0x43:
        inst = 'BIT 0,E';
        code = 'this.bit(this.e & BIT_0);';
        break;
      case 0x44:
        inst = 'BIT 0,H';
        code = 'this.bit(this.h & BIT_0);';
        break;
      case 0x45:
        inst = 'BIT 0,L';
        code = 'this.bit(this.l & BIT_0);';
        break;
      case 0x46:
        inst = 'BIT 0,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_0);';
        break;
      case 0x47:
        inst = 'BIT 0,A';
        code = 'this.bit(this.a & BIT_0);';
        break;
      case 0x48:
        inst = 'BIT 1,B';
        code = 'this.bit(this.b & BIT_1);';
        break;
      case 0x49:
        inst = 'BIT 1,C';
        code = 'this.bit(this.c & BIT_1);';
        break;
      case 0x4A:
        inst = 'BIT 1,D';
        code = 'this.bit(this.d & BIT_1);';
        break;
      case 0x4B:
        inst = 'BIT 1,E';
        code = 'this.bit(this.e & BIT_1);';
        break;
      case 0x4C:
        inst = 'BIT 1,H';
        code = 'this.bit(this.h & BIT_1);';
        break;
      case 0x4D:
        inst = 'BIT 1,L';
        code = 'this.bit(this.l & BIT_1);';
        break;
      case 0x4E:
        inst = 'BIT 1,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_1);';
        break;
      case 0x4F:
        inst = 'BIT 1,A';
        code = 'this.bit(this.a & BIT_1);';
        break;
      case 0x50:
        inst = 'BIT 2,B';
        code = 'this.bit(this.b & BIT_2);';
        break;
      case 0x51:
        inst = 'BIT 2,C';
        code = 'this.bit(this.c & BIT_2);';
        break;
      case 0x52:
        inst = 'BIT 2,D';
        code = 'this.bit(this.d & BIT_2);';
        break;
      case 0x53:
        inst = 'BIT 2,E';
        code = 'this.bit(this.e & BIT_2);';
        break;
      case 0x54:
        inst = 'BIT 2,H';
        code = 'this.bit(this.h & BIT_2);';
        break;
      case 0x55:
        inst = 'BIT 2,L';
        code = 'this.bit(this.l & BIT_2);';
        break;
      case 0x56:
        inst = 'BIT 2,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_2);';
        break;
      case 0x57:
        inst = 'BIT 2,A';
        code = 'this.bit(this.a & BIT_2);';
        break;
      case 0x58:
        inst = 'BIT 3,B';
        code = 'this.bit(this.b & BIT_3);';
        break;
      case 0x59:
        inst = 'BIT 3,C';
        code = 'this.bit(this.c & BIT_3);';
        break;
      case 0x5A:
        inst = 'BIT 3,D';
        code = 'this.bit(this.d & BIT_3);';
        break;
      case 0x5B:
        inst = 'BIT 3,E';
        code = 'this.bit(this.e & BIT_3);';
        break;
      case 0x5C:
        inst = 'BIT 3,H';
        code = 'this.bit(this.h & BIT_3);';
        break;
      case 0x5D:
        inst = 'BIT 3,L';
        code = 'this.bit(this.l & BIT_3);';
        break;
      case 0x5E:
        inst = 'BIT 3,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_3);';
        break;
      case 0x5F:
        inst = 'BIT 3,A';
        code = 'this.bit(this.a & BIT_3);';
        break;
      case 0x60:
        inst = 'BIT 4,B';
        code = 'this.bit(this.b & BIT_4);';
        break;
      case 0x61:
        inst = 'BIT 4,C';
        code = 'this.bit(this.c & BIT_4);';
        break;
      case 0x62:
        inst = 'BIT 4,D';
        code = 'this.bit(this.d & BIT_4);';
        break;
      case 0x63:
        inst = 'BIT 4,E';
        code = 'this.bit(this.e & BIT_4);';
        break;
      case 0x64:
        inst = 'BIT 4,H';
        code = 'this.bit(this.h & BIT_4);';
        break;
      case 0x65:
        inst = 'BIT 4,L';
        code = 'this.bit(this.l & BIT_4);';
        break;
      case 0x66:
        inst = 'BIT 4,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_4);';
        break;
      case 0x67:
        inst = 'BIT 4,A';
        code = 'this.bit(this.a & BIT_4);';
        break;
      case 0x68:
        inst = 'BIT 5,B';
        code = 'this.bit(this.b & BIT_5);';
        break;
      case 0x69:
        inst = 'BIT 5,C';
        code = 'this.bit(this.c & BIT_5);';
        break;
      case 0x6A:
        inst = 'BIT 5,D';
        code = 'this.bit(this.d & BIT_5);';
        break;
      case 0x6B:
        inst = 'BIT 5,E';
        code = 'this.bit(this.e & BIT_5);';
        break;
      case 0x6C:
        inst = 'BIT 5,H';
        code = 'this.bit(this.h & BIT_5);';
        break;
      case 0x6D:
        inst = 'BIT 5,L';
        code = 'this.bit(this.l & BIT_5);';
        break;
      case 0x6E:
        inst = 'BIT 5,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_5);';
        break;
      case 0x6F:
        inst = 'BIT 5,A';
        code = 'this.bit(this.a & BIT_5);';
        break;
      case 0x70:
        inst = 'BIT 6,B';
        code = 'this.bit(this.b & BIT_6);';
        break;
      case 0x71:
        inst = 'BIT 6,C';
        code = 'this.bit(this.c & BIT_6);';
        break;
      case 0x72:
        inst = 'BIT 6,D';
        code = 'this.bit(this.d & BIT_6);';
        break;
      case 0x73:
        inst = 'BIT 6,E';
        code = 'this.bit(this.e & BIT_6);';
        break;
      case 0x74:
        inst = 'BIT 6,H';
        code = 'this.bit(this.h & BIT_6);';
        break;
      case 0x75:
        inst = 'BIT 6,L';
        code = 'this.bit(this.l & BIT_6);';
        break;
      case 0x76:
        inst = 'BIT 6,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_6);';
        break;
      case 0x77:
        inst = 'BIT 6,A';
        code = 'this.bit(this.a & BIT_6);';
        break;
      case 0x78:
        inst = 'BIT 7,B';
        code = 'this.bit(this.b & BIT_7);';
        break;
      case 0x79:
        inst = 'BIT 7,C';
        code = 'this.bit(this.c & BIT_7);';
        break;
      case 0x7A:
        inst = 'BIT 7,D';
        code = 'this.bit(this.d & BIT_7);';
        break;
      case 0x7B:
        inst = 'BIT 7,E';
        code = 'this.bit(this.e & BIT_7);';
        break;
      case 0x7C:
        inst = 'BIT 7,H';
        code = 'this.bit(this.h & BIT_7);';
        break;
      case 0x7D:
        inst = 'BIT 7,L';
        code = 'this.bit(this.l & BIT_7);';
        break;
      case 0x7E:
        inst = 'BIT 7,(HL)';
        code = 'this.bit(this.getUint8(this.getHL()) & BIT_7);';
        break;
      case 0x7F:
        inst = 'BIT 7,A';
        code = 'this.bit(this.a & BIT_7);';
        break;
      case 0x80:
        inst = 'RES 0,B';
        code = 'this.b &= ~BIT_0;';
        break;
      case 0x81:
        inst = 'RES 0,C';
        code = 'this.c &= ~BIT_0;';
        break;
      case 0x82:
        inst = 'RES 0,D';
        code = 'this.d &= ~BIT_0;';
        break;
      case 0x83:
        inst = 'RES 0,E';
        code = 'this.e &= ~BIT_0;';
        break;
      case 0x84:
        inst = 'RES 0,H';
        code = 'this.h &= ~BIT_0;';
        break;
      case 0x85:
        inst = 'RES 0,L';
        code = 'this.l &= ~BIT_0;';
        break;
      case 0x86:
        inst = 'RES 0,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_0);';
        break;
      case 0x87:
        inst = 'RES 0,A';
        code = 'this.a &= ~BIT_0;';
        break;
      case 0x88:
        inst = 'RES 1,B';
        break;
      case 0x89:
        inst = 'RES 1,C';
        break;
      case 0x8A:
        inst = 'RES 1,D';
        break;
      case 0x8B:
        inst = 'RES 1,E';
        break;
      case 0x8C:
        inst = 'RES 1,H';
        break;
      case 0x8D:
        inst = 'RES 1,L';
        break;
      case 0x8E:
        inst = 'RES 1,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_1);';
        break;
      case 0x8F:
        inst = 'RES 1,A';
        break;
      case 0x90:
        inst = 'RES 2,B';
        break;
      case 0x91:
        inst = 'RES 2,C';
        break;
      case 0x92:
        inst = 'RES 2,D';
        break;
      case 0x93:
        inst = 'RES 2,E';
        break;
      case 0x94:
        inst = 'RES 2,H';
        break;
      case 0x95:
        inst = 'RES 2,L';
        break;
      case 0x96:
        inst = 'RES 2,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_2);';
        break;
      case 0x97:
        inst = 'RES 2,A';
        break;
      case 0x98:
        inst = 'RES 3,B';
        break;
      case 0x99:
        inst = 'RES 3,C';
        break;
      case 0x9A:
        inst = 'RES 3,D';
        break;
      case 0x9B:
        inst = 'RES 3,E';
        break;
      case 0x9C:
        inst = 'RES 3,H';
        break;
      case 0x9D:
        inst = 'RES 3,L';
        break;
      case 0x9E:
        inst = 'RES 3,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_3);';
        break;
      case 0x9F:
        inst = 'RES 3,A';
        code = 'this.a &= ~BIT_3;';
        break;
      case 0xA0:
        inst = 'RES 4,B';
        break;
      case 0xA1:
        inst = 'RES 4,C';
        break;
      case 0xA2:
        inst = 'RES 4,D';
        break;
      case 0xA3:
        inst = 'RES 4,E';
        break;
      case 0xA4:
        inst = 'RES 4,H';
        break;
      case 0xA5:
        inst = 'RES 4,L';
        break;
      case 0xA6:
        inst = 'RES 4,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_4);';
        break;
      case 0xA7:
        inst = 'RES 4,A';
        code = 'this.a &= ~BIT_4;';
        break;
      case 0xA8:
        inst = 'RES 5,B';
        break;
      case 0xA9:
        inst = 'RES 5,C';
        break;
      case 0xAA:
        inst = 'RES 5,D';
        break;
      case 0xAB:
        inst = 'RES 5,E';
        break;
      case 0xAC:
        inst = 'RES 5,H';
        break;
      case 0xAD:
        inst = 'RES 5,L';
        break;
      case 0xAE:
        inst = 'RES 5,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_5);';
        break;
      case 0xAF:
        inst = 'RES 5,A';
        break;
      case 0xB0:
        inst = 'RES 6,B';
        break;
      case 0xB1:
        inst = 'RES 6,C';
        break;
      case 0xB2:
        inst = 'RES 6,D';
        break;
      case 0xB3:
        inst = 'RES 6,E';
        break;
      case 0xB4:
        inst = 'RES 6,H';
        break;
      case 0xB5:
        inst = 'RES 6,L';
        break;
      case 0xB6:
        inst = 'RES 6,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_6);';
        break;
      case 0xB7:
        inst = 'RES 6,A';
        code = 'this.a &= ~BIT_6;';
        break;
      case 0xB8:
        inst = 'RES 7,B';
        code = 'this.b &= ~BIT_7;';
        break;
      case 0xB9:
        inst = 'RES 7,C';
        code = 'this.c &= ~BIT_7;';
        break;
      case 0xBA:
        inst = 'RES 7,D';
        code = 'this.d &= ~BIT_7;';
        break;
      case 0xBB:
        inst = 'RES 7,E';
        code = 'this.e &= ~BIT_7;';
        break;
      case 0xBC:
        inst = 'RES 7,H';
        code = 'this.h &= ~BIT_7;';
        break;
      case 0xBD:
        inst = 'RES 7,L';
        code = 'this.l &= ~BIT_7;';
        break;
      case 0xBE:
        inst = 'RES 7,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) & ~BIT_7);';
        break;
      case 0xBF:
        inst = 'RES 7,A';
        code = 'this.a &= ~BIT_7;';
        break;
      case 0xC0:
        inst = 'SET 0,B';
        code = 'this.b |= BIT_0;';
        break;
      case 0xC1:
        inst = 'SET 0,C';
        code = 'this.c |= BIT_0;';
        break;
      case 0xC2:
        inst = 'SET 0,D';
        code = 'this.d |= BIT_0;';
        break;
      case 0xC3:
        inst = 'SET 0,E';
        code = 'this.e |= BIT_0;';
        break;
      case 0xC4:
        inst = 'SET 0,H';
        code = 'this.h |= BIT_0;';
        break;
      case 0xC5:
        inst = 'SET 0,L';
        code = 'this.l |= BIT_0;';
        break;
      case 0xC6:
        inst = 'SET 0,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_0);';
        break;
      case 0xC7:
        inst = 'SET 0,A';
        code = 'this.a |= BIT_0;';
        break;
      case 0xC8:
        inst = 'SET 1,B';
        break;
      case 0xC9:
        inst = 'SET 1,C';
        break;
      case 0xCA:
        inst = 'SET 1,D';
        break;
      case 0xCB:
        inst = 'SET 1,E';
        break;
      case 0xCC:
        inst = 'SET 1,H';
        break;
      case 0xCD:
        inst = 'SET 1,L';
        break;
      case 0xCE:
        inst = 'SET 1,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_1);';
        break;
      case 0xCF:
        inst = 'SET 1,A';
        break;
      case 0xD0:
        inst = 'SET 2,B';
        break;
      case 0xD1:
        inst = 'SET 2,C';
        break;
      case 0xD2:
        inst = 'SET 2,D';
        break;
      case 0xD3:
        inst = 'SET 2,E';
        break;
      case 0xD4:
        inst = 'SET 2,H';
        break;
      case 0xD5:
        inst = 'SET 2,L';
        break;
      case 0xD6:
        inst = 'SET 2,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_2)';
        break;
      case 0xD7:
        inst = 'SET 2,A';
        break;
      case 0xD8:
        inst = 'SET 3,B';
        break;
      case 0xD9:
        inst = 'SET 3,C';
        break;
      case 0xDA:
        inst = 'SET 3,D';
        break;
      case 0xDB:
        inst = 'SET 3,E';
        break;
      case 0xDC:
        inst = 'SET 3,H';
        break;
      case 0xDD:
        inst = 'SET 3,L';
        break;
      case 0xDE:
        inst = 'SET 3,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_3);';
        break;
      case 0xDF:
        inst = 'SET 3,A';
        break;
      case 0xE0:
        inst = 'SET 4,B';
        break;
      case 0xE1:
        inst = 'SET 4,C';
        break;
      case 0xE2:
        inst = 'SET 4,D';
        break;
      case 0xE3:
        inst = 'SET 4,E';
        break;
      case 0xE4:
        inst = 'SET 4,H';
        break;
      case 0xE5:
        inst = 'SET 4,L';
        break;
      case 0xE6:
        inst = 'SET 4,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_4);';
        break;
      case 0xE7:
        inst = 'SET 4,A';
        code = 'this.a |= BIT_4;';
        break;
      case 0xE8:
        inst = 'SET 5,B';
        break;
      case 0xE9:
        inst = 'SET 5,C';
        break;
      case 0xEA:
        inst = 'SET 5,D';
        break;
      case 0xEB:
        inst = 'SET 5,E';
        break;
      case 0xEC:
        inst = 'SET 5,H';
        break;
      case 0xED:
        inst = 'SET 5,L';
        break;
      case 0xEE:
        inst = 'SET 5,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_5);';
        break;
      case 0xEF:
        inst = 'SET 5,A';
        code = 'this.a |= BIT_5;';
        break;
      case 0xF0:
        inst = 'SET 6,B';
        code = 'this.b |= BIT_6;';
        break;
      case 0xF1:
        inst = 'SET 6,C';
        code = 'this.c |= BIT_6;';
        break;
      case 0xF2:
        inst = 'SET 6,D';
        code = 'this.d |= BIT_6;';
        break;
      case 0xF3:
        inst = 'SET 6,E';
        code = 'this.e |= BIT_6;';
        break;
      case 0xF4:
        inst = 'SET 6,H';
        code = 'this.h |= BIT_6;';
        break;
      case 0xF5:
        inst = 'SET 6,L';
        code = 'this.l |= BIT_6;';
        break;
      case 0xF6:
        inst = 'SET 6,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_6);';
        break;
      case 0xF7:
        inst = 'SET 6,A';
        code = 'this.a |= BIT_6;';
        break;
      case 0xF8:
        inst = 'SET 7,B';
        code = 'this.b |= BIT_7;';
        break;
      case 0xF9:
        inst = 'SET 7,C';
        code = 'this.c |= BIT_7;';
        break;
      case 0xFA:
        inst = 'SET 7,D';
        code = 'this.d |= BIT_7;';
        break;
      case 0xFB:
        inst = 'SET 7,E';
        code = 'this.e |= BIT_7;';
        break;
      case 0xFC:
        inst = 'SET 7,H';
        code = 'this.h |= BIT_7;';
        break;
      case 0xFD:
        inst = 'SET 7,L';
        code = 'this.l |= BIT_7;';
        break;
      case 0xFE:
        inst = 'SET 7,(HL)';
        code = 'this.setUint8(this.getHL(), this.getUint8(this.getHL()) | BIT_7);';
        break;
      case 0xFF:
        inst = 'SET 7,A';
        code = 'this.a |= BIT_7;';
        break;
    }

    return {
      opcode: opcode,
      opcodes: opcodesArray,
      inst: inst,
      code: code,
      address: currAddr,
      nextAddress: address
    };
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * @param {number} address
   * @return {Object}
   */
  getED: function(address) {
    var toHex = JSSMS.Utils.toHex;
    var opcode = this.readRom8bit(address);
    var opcodesArray = [opcode];
    var inst = 'Unimplemented 0xED prefixed opcode';
    var currAddr = address;
    var target = null;
    var code = 'throw "Unimplemented 0xED prefixed opcode";';
    var operand = '';
    var location = 0;
    address++;

    switch (opcode) {
      case 0x40:
        inst = 'IN B,(C)';
        code = 'this.b = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.b];';
        break;
      case 0x41:
        inst = 'OUT (C),B';
        code = 'this.port.out(this.c, this.b);';
        break;
      case 0x42:
        inst = 'SBC HL,BC';
        code = 'this.sbc16(this.getBC());';
        break;
      case 0x43:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),BC';
        code = 'this.setUint8(' + operand + ', this.c);' +
            'this.setUint8(' + toHex(location + 1) + ', this.b);';
        address += 2;
        break;
      case 0x44:
      case 0x4C:
      case 0x54:
      case 0x5C:
      case 0x64:
      case 0x6C:
      case 0x74:
      case 0x7C:
        inst = 'NEG';
        // A <- 0-A
        code = 'temp = this.a;' +
            'this.a = 0x00;' +
            'this.sub_a(temp);';
        break;
      case 0x45:
      case 0x4D:
      case 0x55:
      case 0x5D:
      case 0x65:
      case 0x6D:
      case 0x75:
      case 0x7D:
        inst = 'RETN / RETI';
        code = 'this.pc = this.getUint16(this.sp);' +
            'this.sp += 0x02;' +
            'this.iff1 = this.iff2;' +
            'return;';
        address = null;
        break;
      case 0x46:
      case 0x4E:
      case 0x66:
      case 0x6E:
        inst = 'IM 0';
        code = 'this.im = 0x00;';
        break;
      case 0x47:
        inst = 'LD I,A';
        code = 'this.i = this.a;';
        break;
      case 0x48:
        inst = 'IN C,(C)';
        code = 'this.c = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.c];';
        break;
      case 0x49:
        inst = 'OUT (C),C';
        code = 'this.port.out(this.c, this.c);';
        break;
      case 0x4A:
        inst = 'ADC HL,BC';
        code = 'this.adc16(this.getBC());';
        break;
      case 0x4B:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD BC,(' + operand + ')';
        code = 'this.setBC(this.getUint16(' + operand + '));';
        address += 2;
        break;
      case 0x4F:
        inst = 'LD R,A';
        code = 'this.r = this.a;';
        break;
      case 0x50:
        inst = 'IN D,(C)';
        code = 'this.d = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.d];';
        break;
      case 0x51:
        inst = 'OUT (C),D';
        code = 'this.port.out(this.c, this.d);';
        break;
      case 0x52:
        inst = 'SBC HL,DE';
        code = 'this.sbc16(this.getDE());';
        break;
      case 0x53:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),DE';
        code = 'this.setUint8(' + operand + ', this.e);' + // SPl
            'this.setUint8(' + toHex(location + 1) + ', this.d);'; // SPh
        address += 2;
        break;
      case 0x56:
      case 0x76:
        inst = 'IM 1';
        code = 'this.im = 0x01;';
        break;
      case 0x57:
        inst = 'LD A,I';
        code = 'this.a = this.i;' +
            'this.f = (this.f & F_CARRY) | this.SZ_TABLE[this.a] | (this.iff2 ? F_PARITY : 0);';
        break;
      case 0x58:
        inst = 'IN E,(C)';
        code = 'this.e = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.e];';
        break;
      case 0x59:
        inst = 'OUT (C),E';
        code = 'this.port.out(this.c, this.e);';
        break;
      case 0x5A:
        inst = 'ADC HL,DE';
        code = 'this.adc16(this.getDE());';
        break;
      case 0x5B:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD DE,(' + operand + ')';
        code = 'this.setDE(this.getUint16(' + operand + '));';
        address += 2;
        break;
      case 0x5F:
        inst = 'LD A,R';
        if (REFRESH_EMULATION) {
          code = 'this.a = this.r;';
        } else {
          // Note, to fake refresh emulation we use the random number generator
          code = 'this.a = JSSMS.Utils.rndInt(0xFF);';
        }
        code += 'this.f = this.f & F_CARRY | this.SZ_TABLE[this.a] | (this.iff2 ? F_PARITY : 0x00);';
        break;
      case 0x60:
        inst = 'IN H,(C)';
        code = 'this.h = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.h];';
        break;
      case 0x61:
        inst = 'OUT (C),H';
        code = 'this.port.out(this.c, this.h);';
        break;
      case 0x62:
        inst = 'SBC HL,HL';
        code = 'this.sbc16(this.getHL());';
        break;
      case 0x63:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),HL';
        code = 'this.setUint8(' + operand + ', this.l);' + // SPl
            'this.setUint8(' + toHex(location + 1) + ', this.h);'; // SPh
        address += 2;
        break;
      case 0x67:
        inst = 'RRD';
        code = 'var location = this.getHL();' +
            'temp = this.getUint8(location);' +
            // move high 4 of hl to low 4 of hl
            // move low 4 of a to high 4 of hl
            'this.setUint8(location, (temp >> 4) | ((this.a & 0x0F) << 4));' +
            // move 4 lowest bits of hl to low 4 of a
            'this.a = (this.a & 0xF0) | (temp & 0x0F);' +
            'this.f = this.f & F_CARRY | this.SZP_TABLE[this.a];';
        break;
      case 0x68:
        inst = 'IN L,(C)';
        code = 'this.l = this.port.in_(this.c);' +
            'this.f = (this.f & F_CARRY) | this.SZP_TABLE[this.l];';
        break;
      case 0x69:
        inst = 'OUT (C),L';
        code = 'this.port.out(this.c, this.l);';
        break;
      case 0x6A:
        inst = 'ADC HL,HL';
        code = 'this.adc16(this.getHL());';
        break;
      case 0x6B:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD HL,(' + operand + ')';
        code = 'this.setHL(this.getUint16(' + operand + '));';
        address += 2;
        break;
      case 0x6F:
        inst = 'RLD';
        code = 'var location = this.getHL();' +
            'temp = this.getUint8(location);' +
            // move low 4 of hl to high 4 of hl
            // move low 4 of a to low 4 of hl
            'this.setUint8(location, (temp & 0x0F) << 4 | (this.a & 0x0F));' +
            // move high 4 of hl to low 4 of a
            'this.a = (this.a & 0xF0) | (temp >> 4);' +
            'this.f = this.f & F_CARRY | this.SZP_TABLE[this.a];';
        break;
      case 0x71:
        inst = 'OUT (C),0';
        code = 'this.port.out(this.c, 0);';
        break;
      case 0x72:
        inst = 'SBC HL,SP';
        code = 'this.sbc16(this.sp);';
        break;
      case 0x73:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),SP';
        code = 'this.setUint8(' + operand + ', this.sp & 0xFF);' + // SPl
            'this.setUint8(' + toHex(location + 1) + ', this.sp >> 8);'; // SPh
        address += 2;
        break;
      case 0x78:
        inst = 'IN A,(C)';
        code = 'this.a = this.port.in_(this.c);' +
            'this.f = this.f & F_CARRY | this.SZP_TABLE[this.a];';
        break;
      case 0x79:
        inst = 'OUT (C),A';
        code = 'this.port.out(this.c, this.a);';
        break;
      case 0x7A:
        inst = 'ADC HL,SP';
        code = 'this.adc16(this.sp);';
        break;
      case 0x7B:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD SP,(' + operand + ')';
        code = 'this.sp = this.getUint16(' + operand + ');';
        address += 2;
        break;
      case 0xA0:
        inst = 'LDI';
        code = 'temp = this.getUint8(this.getHL());' +
            'this.setUint8(this.getDE(), temp);' +
            'this.decBC();' +
            'this.incDE();' +
            'this.incHL();' +
            'temp = (temp + this.a) & 0xFF;' +
            'this.f = (this.f & 0xC1) | (this.getBC() ? F_PARITY : 0) | (temp & 0x08) | ((temp & 0x02) ? 0x20 : 0);';
        break;
      case 0xA1:
        inst = 'CPI';
        code = 'temp = (this.f & F_CARRY) | F_NEGATIVE;' +
            'this.cp_a(this.getUint8(this.getHL()));' + // sets some flags
            'this.incHL();' +
            'this.decBC();' +
            'temp |= (this.getBC() === 0x00 ? 0x00 : F_PARITY);' +
            'this.f = (this.f & 0xF8) | temp;';
        break;
      case 0xA2:
        inst = 'INI';
        code = 'temp = this.port.in_(this.c);' +
            'this.setUint8(this.getHL(), temp);' +
            'this.b = this.dec8(this.b);' +
            'this.incHL();' +
            'if ((temp & 0x80) === 0x80) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        // undocumented flags not finished.
        break;
      case 0xA3:
        inst = 'OUTI';
        code = 'temp = this.getUint8(this.getHL());' +
            // (C) <- (HL)
            'this.port.out(this.c, temp);' +
            // HL <- HL + 1
            'this.incHL();' +
            // B <- B -1
            'this.b = this.dec8(this.b);' + // Flags in OUTI adjusted in same way as dec b anyway.
            'if (this.l + temp > 0xFF) {' +
            'this.f |= F_CARRY; this.f |= F_HALFCARRY;' +
            '} else {' +
            'this.f &= ~ F_CARRY; this.f &= ~ F_HALFCARRY;' +
            '}' +
            'if ((temp & 0x80) === 0x80) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        break;
      case 0xA8:
        inst = 'LDD';
        break;
      case 0xA9:
        inst = 'CPD';
        break;
      case 0xAA:
        inst = 'IND';
        code = 'temp = this.port.in_(this.c);' +
            'this.setUint8(this.getHL(), temp);' +
            'this.b = this.dec8(this.b);' +
            'this.decHL();' +
            'if ((temp & 0x80) !== 0x00) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        // undocumented flags not finished.
        break;
      case 0xAB:
        inst = 'OUTD';
        code = 'temp = this.getUint8(this.getHL());' +
            // (C) <- (HL)
            'this.port.out(this.c, temp);' +
            // HL <- HL - 1
            'this.decHL();' +
            // B <- B -1
            'this.b = this.dec8(this.b);' + // Flags in OUTI adjusted in same way as dec b anyway.
            'if (this.l + temp > 0xFF) {' +
            'this.f |= F_CARRY; this.f |= F_HALFCARRY;' +
            '} else {' +
            'this.f &= ~ F_CARRY; this.f &= ~ F_HALFCARRY;' +
            '}' +
            'if ((temp & 0x80) === 0x80) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        break;
      case 0xB0:
        // @todo Update to match Z80.
        inst = 'LDIR';
        code = 'this.setUint8(this.getDE(), this.getUint8(this.getHL()));' +
            'this.incDE();' +
            'this.incHL();' +
            'this.decBC();';
        if (ACCURATE_INTERRUPT_EMULATION) {
          target = address - 2;
          code += 'if (this.getBC() !== 0x00) {' +
              'this.tstates -= 0x05;' +
              'this.f |= F_PARITY;' +
              //'this.pc = ' + toHex(target) + ';' +
              'return;' +
              '} else {';
        } else {
          code += 'for (;this.getBC() !== 0x00; this.f |= F_PARITY, this.tstates -= 5) {' +
              'this.setUint8(this.getDE(), this.getUint8(this.getHL()));' +
              'this.incDE();this.incHL();this.decBC();' +
              '}' +
              'if (!(this.getBC() !== 0x00)) {';
        }
        code += 'this.f &= ~ F_PARITY;' +
            '}' +
            'this.f &= ~ F_NEGATIVE; this.f &= ~ F_HALFCARRY;';
        break;
      case 0xB1:
        inst = 'CPIR';
        code = 'temp = (this.f & F_CARRY) | F_NEGATIVE;' +
            'this.cp_a(this.getUint8(this.getHL()));' + // sets zero flag for us
            'this.incHL();' +
            'this.decBC();' +
            'temp |= (this.getBC() === 0x00 ? 0x00 : F_PARITY);';
        // Repeat instruction until a = (hl) or bc === 0
        if (ACCURATE_INTERRUPT_EMULATION) {
          target = address - 2;
          code += 'if ((temp & F_PARITY) !== 0x00 && (this.f & F_ZERO) === 0x00) {' +
              'this.tstates -= 0x05;' +
              'this.pc = ' + toHex(target) + ';' +
              'return;' +
              '}';
        } else {
          code += 'for (;(temp & F_PARITY) !== 0x00 && (this.f & F_ZERO) === 0x00; this.tstates -= 5) {' +
              'temp = (this.f & F_CARRY) | F_NEGATIVE;' +
              'this.cp_a(this.getUint8(this.getHL()));' + // sets zero flag for us
              'this.incHL();' +
              'this.decBC();' +
              'temp |= (this.getBC() === 0x00 ? 0x00 : F_PARITY);' +
              '}';
        }
        code += 'this.f = (this.f & 0xF8) | temp;'; // Sign set by the cp instruction
        break;
      case 0xB2:
        // @todo Apply peephole optimization here.
        target = address - 2;
        inst = 'INIR';
        code = 'temp = this.port.in_(this.c);' +
            'this.setUint8(this.getHL(), temp);' +
            'this.b = this.dec8(this.b);' +
            'this.incHL();' +
            'if (this.b !== 0x00) {' +
            'this.tstates -= 0x05;' +
            //'this.pc = ' + toHex(target) + ';' +
            'return;' +
            //'} else {' +
            //'this.pc++;' +
            '}' +
            'if ((temp & 0x80) === 0x80) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        break;
      case 0xB3:
        inst = 'OTIR';
        code = 'temp = this.getUint8(this.getHL());' +
            // (C) <- (HL)
            'this.port.out(this.c, temp);' +
            // B <- B -1
            'this.b = this.dec8(this.b);' +
            // HL <- HL + 1
            'this.incHL();';
        if (ACCURATE_INTERRUPT_EMULATION) {
          target = address - 2;
          code += 'if (this.b !== 0x00) {' +
              'this.tstates -= 0x05;' +
              //'this.pc = ' + toHex(target) + ';' +
              'return;' +
              '}';
        } else {
          code += 'for (;this.b !== 0x00; this.tstates -= 5) {' +
              'temp = this.getUint8(this.getHL());' +
              // (C) <- (HL)
              'this.port.out(this.c, temp);' +
              // B <- B -1
              'this.b = this.dec8(this.b);' +
              // HL <- HL + 1
              'this.incHL();' +
              '}';
        }
        code += 'if (this.l + temp > 0xFF) {' +
            'this.f |= F_CARRY; this.f |= F_HALFCARRY;' +
            '} else {' +
            'this.f &= ~ F_CARRY; this.f &= ~ F_HALFCARRY;' +
            '}' +
            'if ((temp & 0x80) !== 0x00) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        break;
      case 0xB8:
        inst = 'LDDR';
        break;
      case 0xB9:
        inst = 'CPDR';
        break;
      case 0xBA:
        // @todo Apply peephole optimization here.
        target = address - 2;
        inst = 'INDR';
        code = 'temp = this.port.in_(this.c);' +
            'this.setUint8(this.getHL(), temp);' +
            'this.b = this.dec8(this.b);' +
            'this.decHL();' +
            'if (this.b !== 0x00) {' +
            'this.tstates -= 0x05;' +
            //'this.pc = ' + toHex(target) + ';' +
            'return;' +
            //'} else {' +
            //'this.pc++;' +
            '}' +
            'if ((temp & 0x80) !== 0x00) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        // undocumented flags not finished.
        break;
      case 0xBB:
        // @todo Apply peephole optimization here.
        target = address - 2;
        inst = 'OTDR';
        code = 'temp = this.getUint8(this.getHL());' +
            // (C) <- (HL)
            'this.port.out(this.c, temp);' +
            // B <- B -1
            'this.b = this.dec8(this.b);' +
            // HL <- HL + 1
            'this.decHL();' +
            'if (this.b !== 0x00) {' +
            'this.tstates -= 0x05;' +
            //'this.pc = ' + toHex(target) + ';' +
            'return;' +
            //'} else {' +
            //'this.pc++;' +
            '}' +
            'if (this.l + temp > 0xFF) {' +
            'this.f |= F_CARRY; this.f |= F_HALFCARRY;' +
            '} else {' +
            'this.f &= ~ F_CARRY; this.f &= ~ F_HALFCARRY;' +
            '}' +
            'if ((temp & 0x80) !== 0x00) {' +
            'this.f |= F_NEGATIVE;' +
            '} else {' +
            'this.f &= ~ F_NEGATIVE;' +
            '}';
        break;
    }

    return {
      opcode: opcode,
      opcodes: opcodesArray,
      inst: inst,
      code: code,
      address: currAddr,
      nextAddress: address,
      target: target
    };
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * @param {string} index
   * @param {number} address
   * @return {Object}
   */
  getIndex: function(index, address) {
    var toHex = JSSMS.Utils.toHex;
    var opcode = this.readRom8bit(address);
    var opcodesArray = [opcode];
    var inst = 'Unimplemented 0xDD or 0xFD prefixed opcode';
    var currAddr = address;
    var code = 'throw "Unimplemented 0xDD or 0xFD prefixed opcode";';
    var operand = '';
    var location = 0;
    address++;

    var offset = 0;
    var _inst = {};

    switch (opcode) {
      case 0x09:
        inst = 'ADD ' + index + ',BC';
        code = 'this.set' + index + '(this.add16(this.get' + index + '(), this.getBC()));';
        break;
      case 0x19:
        inst = 'ADD ' + index + ',DE';
        code = 'this.set' + index + '(this.add16(this.get' + index + '(), this.getDE()));';
        break;
      case 0x21:
        operand = toHex(this.readRom16bit(address));
        inst = 'LD ' + index + ',' + operand;
        code = 'this.set' + index + '(' + operand + ');';
        address += 2;
        break;
      case 0x22:
        location = this.readRom16bit(address);
        operand = toHex(location);
        inst = 'LD (' + operand + '),' + index;
        code = 'this.setUint8(' + operand + ', this.' + index.toLowerCase() + 'L);' +
            'this.setUint8(' + toHex(location + 1) + ', this.' + index.toLowerCase() + 'H);';
        address += 2;
        break;
      case 0x23:
        inst = 'INC ' + index;
        code = 'this.inc' + index + '();';
        break;
      case 0x24:
        inst = 'INC ' + index + 'H *';
        break;
      case 0x25:
        inst = 'DEC ' + index + 'H *';
        break;
      case 0x26:
        inst = 'LD ' + index + 'H,' + toHex(this.readRom8bit(address)) + ' *';
        address++;
        break;
      case 0x29:
        inst = 'ADD ' + index + '  ' + index;
        break;
      case 0x2A:
        location = this.readRom16bit(address);
        inst = 'LD ' + index + ' (' + toHex(location) + ')';
        code = 'this.ixL = this.getUint8(' + toHex(location) + ');' +
            'this.ixH = this.getUint8(' + toHex(location + 1) + ');';
        address += 2;
        break;
      case 0x2B:
        inst = 'DEC ' + index;
        code = 'this.dec' + index + '();';
        break;
      case 0x2C:
        inst = 'INC ' + index + 'L *';
        break;
      case 0x2D:
        inst = 'DEC ' + index + 'L *';
        break;
      case 0x2E:
        inst = 'LD ' + index + 'L,' + toHex(this.readRom8bit(address));
        address++;
        break;
      case 0x34:
        offset = this.readRom8bit(address);
        inst = 'INC (' + index + '+' + toHex(offset) + ')';
        code = 'this.incMem(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x35:
        offset = this.readRom8bit(address);
        inst = 'DEC (' + index + '+' + toHex(offset) + ')';
        code = 'this.decMem(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x36:
        offset = this.readRom8bit(address);
        operand = toHex(this.readRom8bit(address + 1));
        inst = 'LD (' + index + '+' + toHex(offset) + '),' + operand;
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', ' + operand + ');';
        address += 2;
        break;
      case 0x39:
        inst = 'ADD ' + index + ' SP';
        code = 'this.set' + index + '(this.add16(this.get' + index + '(), this.sp));';
        break;
      case 0x44:
        inst = 'LD B,' + index + 'H *';
        break;
      case 0x45:
        inst = 'LD B,' + index + 'L *';
        break;
      case 0x46:
        offset = this.readRom8bit(address);
        inst = 'LD B,(' + index + '+' + toHex(offset) + ')';
        code = 'this.b = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x4C:
        inst = 'LD C,' + index + 'H *';
        break;
      case 0x4D:
        inst = 'LD C,' + index + 'L *';
        break;
      case 0x4E:
        offset = this.readRom8bit(address);
        inst = 'LD C,(' + index + '+' + toHex(offset) + ')';
        code = 'this.c = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x54:
        inst = 'LD D,' + index + 'H *';
        break;
      case 0x55:
        inst = 'LD D,' + index + 'L *';
        break;
      case 0x56:
        offset = this.readRom8bit(address);
        inst = 'LD D,(' + index + '+' + toHex(offset) + ')';
        code = 'this.d = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x5C:
        inst = 'LD E,' + index + 'H *';
        break;
      case 0x5D:
        inst = 'LD E,' + index + 'L *';
        break;
      case 0x5E:
        offset = this.readRom8bit(address);
        inst = 'LD E,(' + index + '+' + toHex(offset) + ')';
        code = 'this.e = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x60:
        inst = 'LD ' + index + 'H,B *';
        break;
      case 0x61:
        inst = 'LD ' + index + 'H,C *';
        break;
      case 0x62:
        inst = 'LD ' + index + 'H,D *';
        break;
      case 0x63:
        inst = 'LD ' + index + 'H,E *';
        break;
      case 0x64:
        inst = 'LD ' + index + 'H,' + index + 'H*';
        break;
      case 0x65:
        inst = 'LD ' + index + 'H,' + index + 'L *';
        break;
      case 0x66:
        offset = this.readRom8bit(address);
        inst = 'LD H,(' + index + '+' + toHex(offset) + ')';
        code = 'this.h = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x67:
        inst = 'LD ' + index + 'H,A *';
        break;
      case 0x68:
        inst = 'LD ' + index + 'L,B *';
        break;
      case 0x69:
        inst = 'LD ' + index + 'L,C *';
        break;
      case 0x6A:
        inst = 'LD ' + index + 'L,D *';
        break;
      case 0x6B:
        inst = 'LD ' + index + 'L,E *';
        break;
      case 0x6C:
        inst = 'LD ' + index + 'L,' + index + 'H *';
        break;
      case 0x6D:
        inst = 'LD ' + index + 'L,' + index + 'L *';
        code = '';
        break;
      case 0x6E:
        offset = this.readRom8bit(address);
        inst = 'LD L,(' + index + '+' + toHex(offset) + ')';
        code = 'this.l = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x6F:
        inst = 'LD ' + index + 'L,A *';
        code = 'this.' + index.toLowerCase() + 'L = this.a;';
        break;
      case 0x70:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),B';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.b);';
        address++;
        break;
      case 0x71:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),C';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.c);';
        address++;
        break;
      case 0x72:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),D';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.d);';
        address++;
        break;
      case 0x73:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),E';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.e);';
        address++;
        break;
      case 0x74:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),H';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.h);';
        address++;
        break;
      case 0x75:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),L';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.l);';
        address++;
        break;
      case 0x77:
        offset = this.readRom8bit(address);
        inst = 'LD (' + index + '+' + toHex(offset) + '),A';
        code = 'this.setUint8(this.get' + index + '() + ' + toHex(offset) + ', this.a);';
        address++;
        break;
      case 0x7C:
        inst = 'LD A,' + index + 'H *';
        break;
      case 0x7D:
        inst = 'LD A,' + index + 'L *';
        break;
      case 0x7E:
        offset = this.readRom8bit(address);
        inst = 'LD A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.a = this.getUint8(this.get' + index + '() + ' + toHex(offset) + ');';
        address++;
        break;
      case 0x84:
        inst = 'ADD A,' + index + 'H *';
        break;
      case 0x85:
        inst = 'ADD A,' + index + 'L *';
        break;
      case 0x86:
        offset = this.readRom8bit(address);
        inst = 'ADD A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.add_a(this.getUint8(this.get' + index + '() + ' + toHex(offset) + '));';
        address++;
        break;
      case 0x8C:
        inst = 'ADC A,' + index + 'H *';
        break;
      case 0x8D:
        inst = 'ADC A,' + index + 'L *';
        break;
      case 0x8E:
        offset = this.readRom8bit(address);
        inst = 'ADC A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.adc_a(this.getUint8(this.get' + index + '() + ' + toHex(offset) + '));';
        address++;
        break;
      case 0x94:
        inst = 'SUB ' + index + 'H *';
        break;
      case 0x95:
        inst = 'SUB ' + index + 'L *';
        break;
      case 0x96:
        offset = this.readRom8bit(address);
        inst = 'SUB A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.sub_a(this.getUint8(this.get' + index + '() + ' + toHex(offset) + '));';
        address++;
        break;
      case 0x9C:
        inst = 'SBC A,' + index + 'H *';
        break;
      case 0x9D:
        inst = 'SBC A,' + index + 'L *';
        break;
      case 0x9E:
        offset = this.readRom8bit(address);
        inst = 'SBC A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.sbc_a(this.getUint8(this.get' + index + '() + ' + toHex(offset) + '));';
        address++;
        break;
      case 0xA4:
        inst = 'AND ' + index + 'H *';
        code = 'this.f = this.SZP_TABLE[this.a &= this.' + index.toLowerCase() + 'H];';
        break;
      case 0xA5:
        inst = 'AND ' + index + 'L *';
        code = 'this.f = this.SZP_TABLE[this.a &= this.' + index.toLowerCase() + 'L];';
        break;
      case 0xA6:
        offset = this.readRom8bit(address);
        inst = 'AND A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.f = this.SZP_TABLE[this.a &= this.getUint8(this.get' + index + '() + ' + toHex(offset) + ')] | F_HALFCARRY;';
        address++;
        break;
      case 0xAC:
        inst = 'XOR A ' + index + 'H*';
        code = 'this.f = this.SZP_TABLE[this.a |= this.' + index.toLowerCase() + 'H];';
        break;
      case 0xAD:
        inst = 'XOR A ' + index + 'L*';
        code = 'this.f = this.SZP_TABLE[this.a |= this.' + index.toLowerCase() + 'L];';
        break;
      case 0xAE:
        offset = this.readRom8bit(address);
        inst = 'XOR A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.f = this.SZP_TABLE[this.a ^= this.getUint8(this.get' + index + '() + ' + toHex(offset) + ')];';
        address++;
        break;
      case 0xB4:
        inst = 'OR A ' + index + 'H*';
        code = 'this.f = this.SZP_TABLE[this.a |= this.' + index.toLowerCase() + 'H];';
        break;
      case 0xB5:
        inst = 'OR A ' + index + 'L*';
        code = 'this.f = this.SZP_TABLE[this.a |= this.' + index.toLowerCase() + 'L];';
        break;
      case 0xB6:
        offset = this.readRom8bit(address);
        inst = 'OR A,(' + index + '+' + toHex(offset) + ')';
        code = 'this.f = this.SZP_TABLE[this.a |= this.getUint8(this.get' + index + '() + ' + toHex(offset) + ')];';
        address++;
        break;
      case 0xBC:
        inst = 'CP ' + index + 'H *';
        code = 'this.cp_a(this.' + index.toLowerCase() + 'H);';
        break;
      case 0xBD:
        inst = 'CP ' + index + 'L *';
        code = 'this.cp_a(this.' + index.toLowerCase() + 'L);';
        break;
      case 0xBE:
        offset = this.readRom8bit(address);
        inst = 'CP (' + index + '+' + toHex(offset) + ')';
        code = 'this.cp_a(this.getUint8(this.get' + index + '() + ' + toHex(offset) + '));';
        address++;
        break;
      case 0xCB:
        _inst = this.getIndexCB(index, address);
        inst = _inst.inst;
        code = _inst.code;
        opcodesArray = opcodesArray.concat(_inst.opcodes);
        address = _inst.nextAddress;
        // @todo
        break;
      case 0xE1:
        inst = 'POP ' + index;
        code = 'this.set' + index + '(this.getUint16(this.sp)); this.sp += 0x02;';
        break;
      case 0xE3:
        inst = 'EX SP,(' + index + ')';
        code = 'temp = this.get' + index + '();' +
            'this.set' + index + '(this.getUint16(this.sp));' +
            'this.setUint8(this.sp, temp & 0xFF);' +
            'this.setUint8(this.sp + 1, temp >> 8);';
        break;
      case 0xE5:
        inst = 'PUSH ' + index;
        code = 'this.push2(this.' + index.toLowerCase() + 'H, this.' + index.toLowerCase() + 'L);';
        break;
      case 0xE9:
        inst = 'JP (' + index + ')';
        code = 'this.pc = this.get' + index + '(); return;';
        address = null;
        break;
      case 0xF9:
        inst = 'LD SP,' + index;
        code = 'this.sp = this.get' + index + '();';
        break;
    }

    return {
      opcode: opcode,
      opcodes: opcodesArray,
      inst: inst,
      code: code,
      address: currAddr,
      nextAddress: address
    };
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * @param {string} index
   * @param {number} address
   * @return {Object}
   */
  getIndexCB: function(index, address) {
    // @todo Test me.
    var toHex = JSSMS.Utils.toHex;
    var currAddr = address;
    var location = 'location = this.get' + index + '() + ' + toHex(this.readRom8bit(address++)) + ' & 0xFFFF;';
    var opcode = this.readRom8bit(address);
    var opcodesArray = [opcode];
    var inst = 'Unimplemented 0xDDCB or 0xFDCB prefixed opcode';
    var code = 'throw "Unimplemented 0xDDCB or 0xFDCB prefixed opcode";';
    address++;

    switch (opcode) {
      case 0x00:
        // @todo Test me.
        inst = 'LD B,RLC (' + index + ')';
        code = location +
            'this.b = this.rlc(this.getUint8(location)); this.setUint8(location, this.b);';
        break;
      case 0x01:
        inst = 'LD C,RLC (' + index + ')';
        code = location +
            'this.c = this.rlc(this.getUint8(location)); this.setUint8(location, this.c);';
        break;
      case 0x02:
        inst = 'LD D,RLC (' + index + ')';
        code = location +
            'this.d = this.rlc(this.getUint8(location)); this.setUint8(location, this.d);';
        break;
      case 0x03:
        inst = 'LD E,RLC (' + index + ')';
        break;
      case 0x04:
        inst = 'LD H,RLC (' + index + ')';
        break;
      case 0x05:
        inst = 'LD L,RLC (' + index + ')';
        break;
      case 0x06:
        inst = 'RLC (' + index + ')';
        code = location +
            'this.setUint8(location, this.rlc(this.getUint8(location)));';
        break;
      case 0x07:
        inst = 'LD A,RLC (' + index + ')';
        code = location +
            'this.a = this.rlc(this.getUint8(location)); this.setUint8(location, this.a);';
        break;
      case 0x08:
        inst = 'LD B,RRC (' + index + ')';
        break;
      case 0x09:
        inst = 'LD C,RRC (' + index + ')';
        break;
      case 0x0A:
        inst = 'LD D,RRC (' + index + ')';
        break;
      case 0x0B:
        inst = 'LD E,RRC (' + index + ')';
        break;
      case 0x0C:
        inst = 'LD H,RRC (' + index + ')';
        break;
      case 0x0D:
        inst = 'LD L,RRC (' + index + ')';
        break;
      case 0x0E:
        inst = 'RRC (' + index + ')';
        break;
      case 0x0F:
        inst = 'LD A,RRC (' + index + ')';
        break;
      case 0x10:
        inst = 'LD B,RL (' + index + ')';
        break;
      case 0x11:
        inst = 'LD C,RL (' + index + ')';
        break;
      case 0x12:
        inst = 'LD D,RL (' + index + ')';
        break;
      case 0x13:
        inst = 'LD E,RL (' + index + ')';
        break;
      case 0x14:
        inst = 'LD H,RL (' + index + ')';
        break;
      case 0x15:
        inst = 'LD L,RL (' + index + ')';
        break;
      case 0x16:
        inst = 'RL (' + index + ')';
        break;
      case 0x17:
        inst = 'LD A,RL (' + index + ')';
        break;
      case 0x18:
        inst = 'LD B,RR (' + index + ')';
        break;
      case 0x19:
        inst = 'LD C,RR (' + index + ')';
        break;
      case 0x1A:
        inst = 'LD D,RR (' + index + ')';
        break;
      case 0x1B:
        inst = 'LD E,RR (' + index + ')';
        break;
      case 0x1C:
        inst = 'LD H,RR (' + index + ')';
        break;
      case 0x1D:
        inst = 'LD L,RR (' + index + ')';
        code = location +
            'this.l = this.rr(this.getUint8(location)); this.setUint8(location, this.l);';
        break;
      case 0x1E:
        inst = 'RR (' + index + ')';
        break;
      case 0x1F:
        inst = 'LD A,RR (' + index + ')';
        code = location +
            'this.a = this.rr(this.getUint8(location)); this.setUint8(location, this.a);';
        break;
      case 0x20:
        inst = 'LD B,SLA (' + index + ')';
        break;
      case 0x21:
        inst = 'LD C,SLA (' + index + ')';
        break;
      case 0x22:
        inst = 'LD D,SLA (' + index + ')';
        break;
      case 0x23:
        inst = 'LD E,SLA (' + index + ')';
        break;
      case 0x24:
        inst = 'LD H,SLA (' + index + ')';
        break;
      case 0x25:
        inst = 'LD L,SLA (' + index + ')';
        break;
      case 0x26:
        inst = 'SLA (' + index + ')';
        break;
      case 0x27:
        inst = 'LD A,SLA (' + index + ')';
        break;
      case 0x28:
        inst = 'LD B,SRA (' + index + ')';
        break;
      case 0x29:
        inst = 'LD C,SRA (' + index + ')';
        break;
      case 0x2A:
        inst = 'LD D,SRA (' + index + ')';
        break;
      case 0x2B:
        inst = 'LD E,SRA (' + index + ')';
        break;
      case 0x2C:
        inst = 'LD H,SRA (' + index + ')';
        break;
      case 0x2D:
        inst = 'LD L,SRA (' + index + ')';
        break;
      case 0x2E:
        inst = 'SRA (' + index + ')';
        break;
      case 0x2F:
        inst = 'LD A,SRA (' + index + ')';
        break;
      case 0x30:
        inst = 'LD B,SLL (' + index + ')';
        break;
      case 0x31:
        inst = 'LD C,SLL (' + index + ')';
        break;
      case 0x32:
        inst = 'LD D,SLL (' + index + ')';
        break;
      case 0x33:
        inst = 'LD E,SLL (' + index + ')';
        break;
      case 0x34:
        inst = 'LD H,SLL (' + index + ')';
        break;
      case 0x35:
        inst = 'LD L,SLL (' + index + ')';
        break;
      case 0x36:
        inst = 'SLL (' + index + ') *';
        break;
      case 0x37:
        inst = 'LD A,SLL (' + index + ')';
        break;
      case 0x38:
        inst = 'LD B,SRL (' + index + ')';
        break;
      case 0x39:
        inst = 'LD C,SRL (' + index + ')';
        break;
      case 0x3A:
        inst = 'LD D,SRL (' + index + ')';
        break;
      case 0x3B:
        inst = 'LD E,SRL (' + index + ')';
        break;
      case 0x3C:
        inst = 'LD H,SRL (' + index + ')';
        break;
      case 0x3D:
        inst = 'LD L,SRL (' + index + ')';
        break;
      case 0x3E:
        inst = 'SRL (' + index + ')';
        break;
      case 0x3F:
        inst = 'LD A,SRL (' + index + ')';
        break;
      case 0x40:
      case 0x41:
      case 0x42:
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x46:
      case 0x47:
        inst = 'BIT 0,(' + index + ')';
        break;
      case 0x48:
      case 0x49:
      case 0x4A:
      case 0x4B:
      case 0x4C:
      case 0x4D:
      case 0x4E:
      case 0x4F:
        inst = 'BIT 1,(' + index + ')';
        break;
      case 0x50:
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x55:
      case 0x56:
      case 0x57:
        inst = 'BIT 2,(' + index + ')';
        break;
      case 0x58:
      case 0x59:
      case 0x5A:
      case 0x5B:
      case 0x5C:
      case 0x5D:
      case 0x5E:
      case 0x5F:
        inst = 'BIT 3,(' + index + ')';
        break;
      case 0x60:
      case 0x61:
      case 0x62:
      case 0x63:
      case 0x64:
      case 0x65:
      case 0x66:
      case 0x67:
        inst = 'BIT 4,(' + index + ')';
        break;
      case 0x68:
      case 0x69:
      case 0x6A:
      case 0x6B:
      case 0x6C:
      case 0x6D:
      case 0x6E:
      case 0x6F:
        inst = 'BIT 5,(' + index + ')';
        break;
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x73:
      case 0x74:
      case 0x75:
      case 0x76:
      case 0x77:
        inst = 'BIT 6,(' + index + ')';
        break;
      case 0x78:
      case 0x79:
      case 0x7A:
      case 0x7B:
      case 0x7C:
      case 0x7D:
      case 0x7E:
      case 0x7F:
        inst = 'BIT 7,(' + index + ')';
        break;
      case 0x80:
      case 0x81:
      case 0x82:
      case 0x83:
      case 0x84:
      case 0x85:
      case 0x86:
      case 0x87:
        inst = 'RES 0,(' + index + ')';
        break;
      case 0x88:
      case 0x89:
      case 0x8A:
      case 0x8B:
      case 0x8C:
      case 0x8D:
      case 0x8E:
      case 0x8F:
        inst = 'RES 1,(' + index + ')';
        break;
      case 0x90:
      case 0x91:
      case 0x92:
      case 0x93:
      case 0x94:
      case 0x95:
      case 0x96:
      case 0x97:
        inst = 'RES 2,(' + index + ')';
        break;
      case 0x98:
      case 0x99:
      case 0x9A:
      case 0x9B:
      case 0x9C:
      case 0x9D:
      case 0x9E:
      case 0x9F:
        inst = 'RES 3,(' + index + ')';
        break;
      case 0xA0:
      case 0xA1:
      case 0xA2:
      case 0xA3:
      case 0xA4:
      case 0xA5:
      case 0xA6:
      case 0xA7:
        inst = 'RES 4,(' + index + ')';
        break;
      case 0xA8:
      case 0xA9:
      case 0xAA:
      case 0xAB:
      case 0xAC:
      case 0xAD:
      case 0xAE:
      case 0xAF:
        inst = 'RES 5,(' + index + ')';
        break;
      case 0xB0:
      case 0xB1:
      case 0xB2:
      case 0xB3:
      case 0xB4:
      case 0xB5:
      case 0xB6:
      case 0xB7:
        inst = 'RES 6,(' + index + ')';
        break;
      case 0xB8:
      case 0xB9:
      case 0xBA:
      case 0xBB:
      case 0xBC:
      case 0xBD:
      case 0xBE:
      case 0xBF:
        inst = 'RES 7,(' + index + ')';
        break;
      case 0xC0:
      case 0xC1:
      case 0xC2:
      case 0xC3:
      case 0xC4:
      case 0xC5:
      case 0xC6:
      case 0xC7:
        inst = 'SET 0,(' + index + ')';
        break;
      case 0xC8:
      case 0xC9:
      case 0xCA:
      case 0xCB:
      case 0xCC:
      case 0xCD:
      case 0xCE:
      case 0xCF:
        inst = 'SET 1,(' + index + ')';
        break;
      case 0xD0:
      case 0xD1:
      case 0xD2:
      case 0xD3:
      case 0xD4:
      case 0xD5:
      case 0xD6:
      case 0xD7:
        inst = 'SET 2,(' + index + ')';
        break;
      case 0xD8:
      case 0xD9:
      case 0xDA:
      case 0xDB:
      case 0xDC:
      case 0xDD:
      case 0xDE:
      case 0xDF:
        inst = 'SET 3,(' + index + ')';
        break;
      case 0xE0:
      case 0xE1:
      case 0xE2:
      case 0xE3:
      case 0xE4:
      case 0xE5:
      case 0xE6:
      case 0xE7:
        inst = 'SET 4,(' + index + ')';
        break;
      case 0xE8:
      case 0xE9:
      case 0xEA:
      case 0xEB:
      case 0xEC:
      case 0xED:
      case 0xEE:
      case 0xEF:
        inst = 'SET 5,(' + index + ')';
        break;
      case 0xF0:
      case 0xF1:
      case 0xF2:
      case 0xF3:
      case 0xF4:
      case 0xF5:
      case 0xF6:
      case 0xF7:
        inst = 'SET 6,(' + index + ')';
        break;
      case 0xF8:
      case 0xF9:
      case 0xFA:
      case 0xFB:
      case 0xFC:
      case 0xFD:
      case 0xFE:
      case 0xFF:
        inst = 'SET 7,(' + index + ')';
        break;
    }

    address++;

    return {
      opcode: opcode,
      opcodes: opcodesArray,
      inst: inst,
      code: code,
      address: currAddr,
      nextAddress: address
    };
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * \@todo Use currying here.
   * @param {number} opcode
   * @return {Object}
   */
  getIndexOpIX: function(opcode) {
    return this.getIndex('IX', opcode);
  },


  /**
   * Returns the instruction associated to an opcode for debugging purposes
   *
   * \@todo Use currying here.
   * @param {number} opcode
   * @return {Object}
   */
  getIndexOpIY: function(opcode) {
    return this.getIndex('IY', opcode);
  },


  /**
   * Read a signed value from rom memory.
   *
   * @param {number} address
   * @return {number} Value from memory location.
   */
  readRom8bit: function(address) {
    if (SUPPORT_DATAVIEW) {
      return this.rom[address >> 14].getUint8(address & 0x3FFF);
    } else {
      return this.rom[address >> 14][address & 0x3FFF] & 0xFF;
    }
  },


  /**
   * Read a word (two bytes) from rom memory.
   *
   * @param {number} address
   * @return {number} Value from memory location.
   */
  readRom16bit: function(address) {
    if (SUPPORT_DATAVIEW) {
      if ((address & 0x3FFF) < 0x3FFF) {
        return this.rom[address >> 14].getUint16(address & 0x3FFF, LITTLE_ENDIAN);
      } else {
        return (this.rom[address >> 14].getUint8(address & 0x3FFF)) |
            ((this.rom[++address >> 14].getUint8(address & 0x3FFF)) << 8);
      }
    } else {
      return (this.rom[address >> 14][address & 0x3FFF] & 0xFF) |
          ((this.rom[++address >> 14][address & 0x3FFF] & 0xFF) << 8);
    }
  },


  /**
   * Peephole optimization for port communication.
   * @param {number} port Port number.
   * @return {string}
   */
  peepholePortOut: function(port) {
    //return 'this.port.out(' + JSSMS.Utils.toHex(port) + ', this.a);';

    if (this.main.is_gg && port < 0x07) {
      return '';
    }

    switch (port & 0xC1) {
      case 0x01:
        // Accurate emulation with HCounter
        if (LIGHTGUN) {
          return 'var value = this.a;' +
              'this.port.oldTH = (this.port.getTH(PORT_A) !== 0x00 || this.port.getTH(PORT_B) !== 0x00);' +
              'this.port.writePort(PORT_A, value);' +
              'this.port.writePort(PORT_B, value >> 2);' +
              // Toggling TH latches H Counter
              'if (!this.port.oldTH && (this.port.getTH(PORT_A) !== 0x00 || this.port.getTH(PORT_B) !== 0x00)) {' +
              'this.port.hCounter = this.port.getHCount();' +
              '}';
        } else {
          // Rough emulation of Nationalisation bits
          var code = 'var value = this.a;' +
              'this.port.ioPorts[0] = (value & 0x20) << 1;' +
              'this.port.ioPorts[1] = (value & 0x80);';
          if (this.port.europe === 0) {
            // Not European system
            code += 'this.port.ioPorts[0] = ~this.port.ioPorts[0];' +
                'this.port.ioPorts[1] = ~this.port.ioPorts[1];';
          }
          return code;
        }
        break;

      // 0xBE VDP Data port
      case 0x80:
        return 'this.vdp.dataWrite(this.a);';

      // 0xBD / 0xBF VDP Control port (Mirrored at two locations)
      case 0x81:
        return 'this.vdp.controlWrite(this.a);';

      // 0x7F: PSG
      case 0x40:
      case 0x41:
        if (this.main.soundEnabled) {
          return 'this.psg.write(this.a);';
        }
        break;
    }

    return '';
  },


  /**
   * Peephole optimization for port communication.
   * @param {number} port Port number.
   * @return {string}
   */
  peepholePortIn: function(port) {
    //return 'this.a = this.port.in_(' + JSSMS.Utils.toHex(port) + ');';

    // Game Gear Serial Ports (not fully emulated)
    if (this.main.is_gg && port < 0x07) {
      switch (port) {
        // Game Gear (Start Button and Nationalisation)
        case 0x00:
          return 'this.a = (this.port.keyboard.ggstart & 0xBF) | this.port.europe;';

        // GG Serial Communication Ports -
        // Return 0 for now as "OutRun" gets stuck in a loop by returning 0xFF
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
          return 'this.a = 0x00;';
        case 0x06:
          return 'this.a = 0xFF;';
      }
    }

    switch (port & 0xC1) {
      // 0x7E - Vertical Port
      case 0x40:
        return 'this.a = this.vdp.getVCount();';

      // 0x7F - Horizontal Port
      case 0x41:
        return 'this.a = this.port.hCounter;';

      // VDP Data port
      case 0x80:
        return 'this.a = this.vdp.dataRead();';

      // VDP Control port
      case 0x81:
        return 'this.a = this.vdp.controlRead();';

      // 0xC0 / 0xDC - I/O Port A
      case 0xC0:
        return 'this.a = this.port.keyboard.controller1;';

      // 0xC1 / 0xDD - I/O Port B and Misc
      case 0xC1:
        if (LIGHTGUN) {
          return 'if (this.port.keyboard.lightgunClick)' +
              'this.port.lightPhaserSync();' +
              'this.a = (this.port.keyboard.controller2 & 0x3F) | (this.port.getTH(PORT_A) !== 0x00 ? 0x40 : 0x00) | (this.port.getTH(PORT_B) !== 0x00 ? 0x80 : 0x00);';
        } else {
          return 'this.a = (this.port.keyboard.controller2 & 0x3F) | this.port.ioPorts[0] | this.port.ioPorts[1];';
        }
    }

    // Default Value is 0xFF
    return 'this.a = 0xFF;';
  }
};


/**
 * \@todo Move elsewhere.
 * @param {Object.<string,*>} options
 */
function Instruction(options) {
  var toHex = JSSMS.Utils.toHex;

  var defaultInstruction = {
    address: 0,
    hexAddress: '',
    opcode: 0,
    opcodes: [],
    inst: '',
    code: '',
    nextAddress: null,
    target: null,
    isJumpTarget: false,
    jumpTargetNb: 0, // Number of instructions targeting there.
    label: ''
    // Memory can be registry or offset, read or write mode, 8 or 16 bit.
    /*memory: null,

     srcRegs: {},
     dstRegs: {}*/
  };
  var prop;
  var hexOpcodes = '';

  // Merge passed values.
  for (prop in defaultInstruction) {
    if (options[prop] !== undefined) {
      defaultInstruction[prop] = options[prop];
    }
  }

  // Computing additional properties
  defaultInstruction.hexAddress = toHex(defaultInstruction.address);
  if (defaultInstruction.opcodes.length) {
    hexOpcodes = defaultInstruction.opcodes
      .map(toHex)
      .join(' ') +
        ' ';
  }
  defaultInstruction.label = defaultInstruction.hexAddress + ' ' +
      hexOpcodes +
      defaultInstruction.inst;

  return defaultInstruction;
}
