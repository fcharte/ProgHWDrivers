  ORG 100h

loop:
  MOV AH,0
  INT 16h
 
  CMP AL, 27
  JZ exit

  MOV AH,0ah
  MOV CX,1
  INT 10h

  JMP loop

exit:
  MOV AH,4Ch
  INT 21h

