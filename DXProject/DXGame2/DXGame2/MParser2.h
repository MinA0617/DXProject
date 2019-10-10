//#pragma once
//#include "MStd.h"
//#include "MKeyAnimationMgr.h"
////----------------------------------------------------------------------------------------------------------------------------------------------------------------
////	ASE File Parser
////		*SCENE										// ���� ����
////			*SCENE_FILENAME "?.max"					// ? �κ��� �ִϸ��̼��� �̸����� �޴´�.
////			*SCENE_FIRSTFRAME *SCENE_LASTFRAME		// SCENE_LASTFRAME-SCENE_FIRSTFRAME�� �ִϸ��̼��� ����
////			*SCENE_FRAMESPEED						// �ʴ� ������
////			*SCENE_TICKSPERFRAME					// �����Ӵ� ƽ (max�� 1�ʴ� 4800ƽ�� ���´�)
////			*SCENE_BACKGROUND_STATIC				// ����
////			*SCENE_AMBIENT_STATIC					// ����
////		*GEOMOBJECT									// ���� ����
////			*NODE_NAME								// �𵨸�
////			*NODE_PARENT							// �θ��� �𵨸�. ��Ʈ���� ����
////			*NODE_TM								// ���� �������
////				*NODE_NAME							// ����
////				*INHERIT_POS						// 0 ��ũ ���� 1 ��ũ ���� (Direct ���� x,z,y)
////				*INHERIT_ROT						// 0 ��ũ ���� 1 ��ũ ����
////				*INHERIT_SCL						// 0 ��ũ ���� 1 ��ũ ����
////				*TM_ROW0~3							// ������ǥ ���. �ڿ� 0�ϳ� �� ������ 4*4 ����� �ȴ�.
////				*TM_POS								// ���� ������ ( ������ 0���� ������ )
////				*TM_ROTAXIS							// ���� ������ �� ȸ�� �� ( �������� ������ 0���� ������ )
////				*TM_ROTANGLE						// ���� ������ �� ȸ�� �� ( ���Ȱ� ������ 0���� ������ )
////				*TM_SCALE							// ���� ������ ( ������ 1�� ������ )
////				*TM_SCALEAXIS						// ���� ������ �� ������ �� ( �������� ������ 0���� ������ )
////				*TM_SCALEAXISANG					// ���� ������ �� ������ �� ( ���Ȱ� ������ 0���� ������ )
////			*MESH
////				*TIMEVALUE							// ����
////				*MESH_NUMVERTEX						// ���ؽ� ����
////				*MESH_NUMFACES						// ���̽� ����
////				*MESH_NUMTVERTEX					// UV���ؽ� ����
////					*MESH_VERTEX					// ���ؽ� ���� (�ѹ�, x,z,y) ��
////					*MESH_FACE						// �ѹ�, A = v, B = vt, C = vn, �����̱׷�� ���긶�׸���ID�� ��������
////					*MESH_TVERT						// u,v,w �� w�� �����ϰ� v = 1-v�� �ٲ�����
////				*MESH_NORMALS						// �븻
////					*MESH_FACENORMAL				// ���̽��븻
////						*MESH_VERTEXNORMAL			// ���̽��� ���� ���� 3���� �븻
////		*TM_ANIMATION								// �ִϸ��̼� ����
////			*NODE_NAME "Bone003"					// �ִϸ��̼� ������Ʈ ����
////			*CONTROL_POS_TRACK						
////				*CONTROL_POS_SAMPLE					// ƽ / ������			
////			*CONTROL_ROT_TRACK
////				*CONTROL_ROT_SAMPLE					// ƽ / ���ʹϾ�
////			*CONTROL_SCALE_TRACK					
////				*CONTROL_SCALE_SAMPLE				// ƽ / ������ / ����
////----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//struct int3
//{
//	int v1, v2, v3;
//	int3()
//	{}
//	int3(int iv1, int iv2, int iv3)
//	{
//		v1 = iv1;
//		v2 = iv2;
//		v3 = iv3;
//	}
//};
//
//class MParser2
//{
//public:
//	bool	LoadKeyAnimation(M_STR filename);
//private:
//	vector<string> m_wordlist;
//	bool CreateData(M_STR name);
//public:
//	MParser2();
//	~MParser2();
//};
//
