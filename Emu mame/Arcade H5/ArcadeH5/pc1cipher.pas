unit pc1cipher;

// PC1 CIPHER with 128-bit keys //

interface

uses
  Windows, Messages, SysUtils, Classes, Controls;

type pc1key=string;

function pc1encode(text:string; key:pc1key):string;
function pc1decode(text:string; key:pc1key):string;

implementation

var
	ax,bx,cx,dx,si,tmp,x1a2,res,i,inter,cfc,cfd,compte,j,k,l : Word;
	x1a0  : array[0..7] of Word;
	cle   : array[0..15] of char;
	cry   : array[0..33000] of char;
  newkey: string;


procedure code;
begin
  dx:= x1a2+i;
	ax:= x1a0[i];
  cx:= $015a;
  bx:= $4e35;
  tmp:= ax;
	ax:= si;
  si:= tmp;
	tmp:= ax;
	ax:= dx;
	dx:= tmp;
  if (ax <> 0) then ax:= ax*bx;
  tmp:= ax;
  ax:= cx;
  cx:= tmp;
  if (ax <> 0) then
  begin
    ax:= ax*si;
    cx:= ax+cx;
  end;
	tmp:= ax;
  ax:= si;
  si:= tmp;
  ax:= ax*bx;
  dx:= cx+dx;
	ax:= ax+1;
  x1a2:= dx;
	x1a0[i]:= ax;
  res:= ax xor dx;
	i:= i+1;
end;

Procedure Assemble;
begin
	x1a0[0]:= ( ord(cle[0])*256 ) + ord(cle[1]);
  code;
  inter:= res;

	x1a0[1]:= x1a0[0] xor ( (ord(cle[2])*256) + ord(cle[3]) );
	code;
	inter:= inter xor res;

	x1a0[2]:= x1a0[1] xor ( (ord(cle[4])*256) + ord(cle[5]) );
	code;
	inter:= inter xor res;

	x1a0[3]:= x1a0[2] xor ( (ord(cle[6])*256) + ord(cle[7]) );
	code;
	inter:= inter xor res;

	x1a0[4]:= x1a0[3] xor ( (ord(cle[8])*256) + ord(cle[9]) );
	code;
	inter:= inter xor res;

        x1a0[5]:= x1a0[4] xor ( (ord(cle[10])*256) + ord(cle[11]) );
        code;
        inter:= inter xor res;

        x1a0[6]:= x1a0[5] xor ( (ord(cle[12])*256) + ord(cle[13]) );
        code;
        inter:= inter xor res;

        x1a0[7]:= x1a0[6] xor ( (ord(cle[14])*256) + ord(cle[15]) );
        code;
        inter:= inter xor res;

	i:= 0;
end;



procedure Crypt(ThisCle, Buffer: PChar; BufferLength: Integer);
// The buffer contains the message to encrypt. No need to be null-termindated,
// since its length is explicitly specified.
// ThisCle contains the password, 16 characters at max.
var
	Rep: Char;
	c, d, e: Byte;
begin
	// Some initializations
	ZeroMemory(@Cry, SizeOf(Cry));
	ZeroMemory(@Cle, SizeOf(Cle));
	StrCopy(Cle, ThisCle);
	si:=0;
	x1a2:=0;
	i:=0;

	for j:=0 to BufferLength-1 do begin
		c:= ord(Buffer[j]);      { c = first byte to crypt}
		Assemble;
		cfc:= inter shr 8;
		cfd:= inter and 255;
		for compte:= 0 to 15 do
		 cle[compte]:= chr(ord(cle[compte]) xor c);
		c:= c xor (cfc xor cfd);

		d:= c shr 4; e:= c and 15;
		Case d of
			0 : rep:= 'a';
			1 : rep:= 'b';
			2 : rep:= 'c';
			3 : rep:= 'd';
			4 : rep:= 'e';
			5 : rep:= 'f';
			6 : rep:= 'g';
			7 : rep:= 'h';
			8 : rep:= 'i';
			9 : rep:= 'j';
			10: rep:= 'k';
			11: rep:= 'l';
			12: rep:= 'm';
			13: rep:= 'n';
			14: rep:= 'o';
			15: rep:= 'p';
		end;
		cry[j*2]:=rep; // contains the first letter
		// shorter: cry[j*2]:=Char($61+d); // "case" and "rep" no longer needed


		Case e of
			0 : rep:= 'a';
			1 : rep:= 'b';
			2 : rep:= 'c';
			3 : rep:= 'd';
			4 : rep:= 'e';
			5 : rep:= 'f';
			6 : rep:= 'g';
			7 : rep:= 'h';
			8 : rep:= 'i';
			9 : rep:= 'j';
			10: rep:= 'k';
			11: rep:= 'l';
			12: rep:= 'm';
			13: rep:= 'n';
			14: rep:= 'o';
			15: rep:= 'p';
		end;
		cry[j*2+1]:=rep; // contains here the second letter
		// shorter: cry[j*2+1]:=Char($61+e); // "case" and "rep" no longer needed
	end;
end;

Procedure Decrypt(ThisCle, Buffer: PChar; BufferLength: Integer);

     var
	Rep: Char;
	c, d, e: Byte;
begin
	// Some initializations
	ZeroMemory(@Cry, SizeOf(Cry));
	ZeroMemory(@Cle, SizeOf(Cle));
	StrCopy(Cle, ThisCle);
	si:=0;
	x1a2:=0;
	i:=0;
        j:=0;
        l:=0;

	while j<BufferLength-1 do begin
        //(j:=0 to BufferLength-1 do begin

          rep:= Buffer[j];
          case rep of
            'a' : d:= 0;
            'b' : d:= 1;
            'c' : d:= 2;
            'd' : d:= 3;
            'e' : d:= 4;
            'f' : d:= 5;
            'g' : d:= 6;
            'h' : d:= 7;
            'i' : d:= 8;
            'j' : d:= 9;
            'k' : d:= 10;
            'l' : d:= 11;
            'm' : d:= 12;
            'n' : d:= 13;
            'o' : d:= 14;
            'p' : d:= 15;
          end;

          d:= d shl 4;
          j:=j+1;

          rep:= Buffer[j];      { rep =  second letter }
          Case rep of
            'a' : e:= 0;
            'b' : e:= 1;
            'c' : e:= 2;
            'd' : e:= 3;
            'e' : e:= 4;
            'f' : e:= 5;
            'g' : e:= 6;
            'h' : e:= 7;
            'i' : e:= 8;
            'j' : e:= 9;
            'k' : e:= 10;
            'l' : e:= 11;
            'm' : e:= 12;
            'n' : e:= 13;
            'o' : e:= 14;
            'p' : e:= 15;
          end;
          c:= d + e;
          Assemble;
          cfc:= inter shr 8;
          cfd:= inter and 255;

          c:= c xor (cfc xor cfd);

          for compte:= 0 to 15 do
            cle[compte]:= chr(ord(cle[compte]) xor c);

          // Note : c contains the decrypted byte
          cry[l]:=chr(c);
          j:=j+1;
          l:=l+1;
    end;

end;


function pc1encode(text:string; key:pc1key):string;
var
	  Buf : PChar;
 Bufkey : Pchar;
keysize : Integer;
	 Size : Integer;
begin
Size := Length(text);
if Size<=0 then begin Result:='Please enter text'; exit; end;

keysize := Length(key);
if keysize<=0 then begin Result:='Please enter key'; exit; end;
if keysize > 16 then begin Result:='Key must be max 16 chars'; exit; end;

//GetMem(Buf,Size+1);
Buf := PChar(text);

//GetMem(Bufkey, keysize+1);
Bufkey := PChar(key);

crypt(Bufkey, buf, Size);

Result := WideString(Cry);

//FreeMem(buf);
//FreeMem(Bufkey);

end;


function pc1decode(text:string; key:pc1key):string;
var
	  Buf : PChar;
 Bufkey : Pchar;
keysize : Integer;
	 Size : Integer;
begin
Size := Length(text);
if Size<=0 then begin Result:='Please enter text'; exit; end;

keysize := Length(key);
if keysize<=0 then begin Result:='Please enter key'; exit; end;
if keysize > 16 then begin Result:='Key must be max 16 chars'; exit; end;

//GetMem(Buf,Size+1);
Buf := PChar(text);

//GetMem(Bufkey, keysize+1);
Bufkey := PChar(key);

//crypt(Bufkey, buf, Size);
decrypt(Bufkey, buf, Size);

Result := WideString(Cry);

end;

end.
