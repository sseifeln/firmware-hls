#include "ProjectionRouterWrapper.h"

void ProjectionRouterWrapper(
			     TProjData inprojdata1[MemDepth],
			     TProjData inprojdata2[MemDepth],
			     TProjData inprojdata3[MemDepth],
			     TProjData inprojdata4[MemDepth],
			     TProjData inprojdata5[MemDepth],
			     TProjData inprojdata6[MemDepth],
			     TProjData inprojdata7[MemDepth],
			     TProjData inprojdata8[MemDepth],
			     // more
				 unsigned int inprojnumber1,
				 unsigned int inprojnumber2,
				 unsigned int inprojnumber3,
				 unsigned int inprojnumber4,
				 unsigned int inprojnumber5,
				 unsigned int inprojnumber6,
				 unsigned int inprojnumber7,
				 unsigned int inprojnumber8,

			     AllProjData outallproj[MemDepth],
			     VMProjData outvmprojphi1[MemDepth],
			     VMProjData outvmprojphi2[MemDepth],
			     VMProjData outvmprojphi3[MemDepth],
			     VMProjData outvmprojphi4[MemDepth],
			     VMProjData outvmprojphi5[MemDepth],
			     VMProjData outvmprojphi6[MemDepth],
			     VMProjData outvmprojphi7[MemDepth],
			     VMProjData outvmprojphi8[MemDepth],
			     VMProjData outvmprojphi9[MemDepth],
			     VMProjData outvmprojphi10[MemDepth],
			     VMProjData outvmprojphi11[MemDepth],
			     VMProjData outvmprojphi12[MemDepth]
			     )
{
  static ProjectionRouter aPR;
  
  aPR.addInputs(inprojdata1, inprojdata2, inprojdata3, inprojdata4, 
		inprojdata5, inprojdata6, inprojdata7, inprojdata8,
		inprojnumber1, inprojnumber2, inprojnumber3, inprojnumber4,
		inprojnumber5, inprojnumber6, inprojnumber7, inprojnumber8);
  aPR.addOutputs(outallproj,
		 outvmprojphi1, outvmprojphi2, outvmprojphi3, 
		 outvmprojphi4, outvmprojphi5, outvmprojphi6, 
		 outvmprojphi7, outvmprojphi8, outvmprojphi9, 
		 outvmprojphi10, outvmprojphi11, outvmprojphi12);

  aPR.execute();
}
