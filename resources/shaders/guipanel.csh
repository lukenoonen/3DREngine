            �$  !�     �s��9X��L�`O�=                                                 `   �  �$                                                                                                                                        ^                                                                                                           ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����                                                                                                ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������                      	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      �$              �u��69���u��69��j�}�m�i                                                                                                                                      	                                                                         
              \�                                            ����                                  �   ��������������������������������                ����                                 \�                                           ����                                  �   ��������������������������������                ����                                 \�                                @          ����                                   �   ��������������������������������                ����                       9          \�                                 @          ����                                   �   ��������������������������������                ����                    
   F          Q�                          ����    ������������                                      �   ����    ������������������������                                           Q          Q�                                �           ����                                   �   ��������������������������������                ����                                       u_matModel u_matProjectionView u_matProjectionViewLocked u_matTIModel u_vecColor u_vecViewPos                       P�                     a_vecPos                   R�                          ��������gl_Position                                               v_vecColour               �������   �                         
              �������   �                          modelBuffer viewBuffer                   ����P�                   a_vecPos                       R�                    v_vecColour  p  !!NVvp5.0
OPTION NV_internal;
OPTION NV_bindless_texture;
CBUFFER buf0[] = { program.buffer[0] };
CBUFFER buf1[] = { program.buffer[1] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..0] };
TEMP R0, R1, R2;
TEMP T;
LDC.F32X4 R0, buf1[32];
MUL.F32 R2, vertex.attrib[0].y, R0;
LDC.F32X4 R0, buf1[0];
MAD.F32 R0, vertex.attrib[0].x, R0, R2;
LDC.F32X4 R1, buf1[48];
ADD.F32 R1, R0, R1;
ADD.F32 R2, R1, {0, 0, 0, 0}.x;
LDC.F32X4 R0, buf0[16];
MUL.F32 R1, R2.y, R0;
LDC.F32X4 R0, buf0[0];
MAD.F32 R1, R2.x, R0, R1;
LDC.F32X4 R0, buf0[32];
MAD.F32 R1, R2.z, R0, R1;
LDC.F32X4 R0, buf0[48];
MAD.F32 result.position, R2.w, R0, R1;
END
                ��������������������������������                                                                                                                                        ���������   !!NVfp5.0
OPTION NV_internal;
OPTION NV_bindless_texture;
PARAM c[1] = { program.local[0] };
TEMP T;
OUTPUT result_color0 = result.color;
MOV.F result_color0.xyz, c[0];
MOV.F result_color0.w, {1, 0, 0, 0}.x;
END
                                                                                                                                                   ��������������������������������                                                                                                                                        ��������                        