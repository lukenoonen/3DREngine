            �*  !�     �����_��I��ד                                                 `   �  �*                                                                             	   	   	                                                     �                                                                                                           ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����                                                                                                ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           �*              �u��69���u��69��2""oڿr�                                                                                                                                                                                                                                                   ����    ������������                                      �      ����������������������������                        ����          
             \�                                            ����                                  �   ��������������������������������                ����                                 \�                                           ����                                  �   ��������������������������������                ����                       -          \�                                @          ����                                   �   ��������������������������������                ����                       G          \�                                 @          ����                                   �   ��������������������������������                ����                       T          ^�                         ����    ������������                                     �   ����    ������������������������                       г���          
   \          Q�                          ����    ������������                                     �   ����   ������������������������                       �����             g          P�                          ����    ������������                                     �       ����������������������������                                          s          Q�                                �           ����                                   �   ��������������������������������                ����                                                                              u_flTextScale u_matModel u_matProjectionView u_matProjectionViewLocked u_matTIModel u_sFont u_vecColor u_vecOffset u_vecViewPos                       P�                        	                 P�                     a_vecPos a_vecTexCoords                      P�                          ��������             R�                          ��������v_vecTexCoords gl_Position                                               v_vecColour               �������   �                         
              �������   �                          modelBuffer viewBuffer                   ����P�                      	             ����P�                   a_vecPos a_vecTexCoords                   ����P�                   v_vecTexCoords                       R�                    v_vecColour  :  !!NVvp5.0
OPTION NV_internal;
OPTION NV_bindless_texture;
PARAM c[2] = { program.local[0..1] };
CBUFFER buf0[] = { program.buffer[0] };
CBUFFER buf1[] = { program.buffer[1] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..1] };
OUTPUT result_attrib[] = { result.attrib[0..0] };
TEMP R0, R1, R2, R3;
TEMP T;
LDC.F32X4 R0, buf1[32];
MAD.F32 R1.x, vertex.attrib[0].y, c[1], -c[0].y;
MUL.F32 R2, R1.x, R0;
LDC.F32X4 R1, buf1[48];
LDC.F32X4 R0, buf1[0];
MAD.F32 R3.x, vertex.attrib[0], c[1], c[0];
MAD.F32 R0, R3.x, R0, R2;
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
MOV.F result.attrib[0].xy, vertex.attrib[1];
END
                                                                                                                                               ��������������������������������                                                                                                                                        ��������  !!NVfp5.0
OPTION NV_internal;
OPTION NV_gpu_program_fp64;
OPTION NV_bindless_texture;
PARAM c[2] = { program.local[0..1] };
ATTRIB fragment_attrib[] = { fragment.attrib[0..0] };
TEMP R0;
LONG TEMP D0;
TEMP T;
TEMP RC;
SHORT TEMP HC;
OUTPUT result_color0 = result.color;
PK64.U D0.x, c[0];
TEX.F R0.x, fragment.attrib[0], handle(D0.x), 2D;
SLT.F32 R0.y, R0.x, {0.5, 0, 0, 0}.x;
TRUNC.U.CC HC.x, R0.y;
IF NE.x;
MOV.U.CC RC.x, {1, 0, 0, 0};
KIL NE.x;
ENDIF;
MOV.F result_color0.w, R0.x;
MOV.F result_color0.xyz, c[1];
END
                                                                                                                                                  �������������������������������                                                                                                                                       ��������                        