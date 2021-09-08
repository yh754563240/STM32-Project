#include "sys.h"
#include "stdio.h"
  
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断
//__asm void INTX_DISABLE(void)
//{
//	CPSID I;		  
//}
//开启所有中断
//__asm void INTX_ENABLE(void)
//{
//	CPSIE I;		  
//}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

	void SoftReset(void) 
	
	{ 
//	__set_FAULTMASK(1);      // 关闭所有中端 
	__disable_irq();
	NVIC_SystemReset();// 复位 
	}
