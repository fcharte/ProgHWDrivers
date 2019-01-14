  ORG 100h

loop:
  IN AL,60h
 
  CMP AL,1
  JZ exit

  MOV AH,0ah
  MOV CX,1
  INT 10h

  IN AL, 61h
  OR AL, 80h
  OUT 61h, AL
  AND AL, 7Fh
  OUT 61h, AL

  JMP loop

exit:
  MOV AH,4Ch
  INT 21h

