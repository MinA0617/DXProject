//#pragma once
//#include "MStd.h"
//#include "MKeyAnimationMgr.h"
////----------------------------------------------------------------------------------------------------------------------------------------------------------------
////	ASE File Parser
////		*SCENE										// 씬의 정보
////			*SCENE_FILENAME "?.max"					// ? 부분을 애니메이션의 이름으로 받는다.
////			*SCENE_FIRSTFRAME *SCENE_LASTFRAME		// SCENE_LASTFRAME-SCENE_FIRSTFRAME는 애니메이션의 길이
////			*SCENE_FRAMESPEED						// 초당 프레임
////			*SCENE_TICKSPERFRAME					// 프레임당 틱 (max는 1초당 4800틱을 갖는다)
////			*SCENE_BACKGROUND_STATIC				// 무시
////			*SCENE_AMBIENT_STATIC					// 무시
////		*GEOMOBJECT									// 모델의 정보
////			*NODE_NAME								// 모델명
////			*NODE_PARENT							// 부모의 모델명. 루트에는 없음
////			*NODE_TM								// 모델의 월드행렬
////				*NODE_NAME							// 무시
////				*INHERIT_POS						// 0 링크 적용 1 링크 무시 (Direct 기준 x,z,y)
////				*INHERIT_ROT						// 0 링크 적용 1 링크 무시
////				*INHERIT_SCL						// 0 링크 적용 1 링크 무시
////				*TM_ROW0~3							// 월드좌표 행렬. 뒤에 0하나 더 붙히면 4*4 행렬이 된다.
////				*TM_POS								// 월드 포지션 ( 뽑을때 0으로 해주자 )
////				*TM_ROTAXIS							// 월드 임의의 축 회전 축 ( 단위벡터 뽑을때 0으로 해주자 )
////				*TM_ROTANGLE						// 월드 임의의 축 회전 값 ( 라디안값 뽑을때 0으로 해주자 )
////				*TM_SCALE							// 월드 스케일 ( 뽑을때 1로 해주자 )
////				*TM_SCALEAXIS						// 월드 임의의 축 스케일 축 ( 단위벡터 뽑을때 0으로 해주자 )
////				*TM_SCALEAXISANG					// 월드 임의의 축 스케일 값 ( 라디안값 뽑을때 0으로 해주자 )
////			*MESH
////				*TIMEVALUE							// 무시
////				*MESH_NUMVERTEX						// 버텍스 갯수
////				*MESH_NUMFACES						// 페이스 갯수
////				*MESH_NUMTVERTEX					// UV버텍스 갯수
////					*MESH_VERTEX					// 버텍스 정보 (넘버, x,z,y) 순
////					*MESH_FACE						// 넘버, A = v, B = vt, C = vn, 스무싱그룹과 서브마테리얼ID는 무시하자
////					*MESH_TVERT						// u,v,w 순 w는 무시하고 v = 1-v로 바꿔주자
////				*MESH_NORMALS						// 노말
////					*MESH_FACENORMAL				// 페이스노말
////						*MESH_VERTEXNORMAL			// 페이스에 속한 정점 3개의 노말
////		*TM_ANIMATION								// 애니메이션 정보
////			*NODE_NAME "Bone003"					// 애니메이션 오브젝트 네임
////			*CONTROL_POS_TRACK						
////				*CONTROL_POS_SAMPLE					// 틱 / 포지션			
////			*CONTROL_ROT_TRACK
////				*CONTROL_ROT_SAMPLE					// 틱 / 쿼터니언
////			*CONTROL_SCALE_TRACK					
////				*CONTROL_SCALE_SAMPLE				// 틱 / 스케일 / 무시
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
