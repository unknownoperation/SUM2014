/* FILENAME: SHADER.H
 * PROGRAMMER: OK2
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* ������� ������������� ��������� ��������� */
UINT OK2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* ������� �������� ��������� ��������� */
VOID OK2_ShadProgClose( UINT ProgId );

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */
