/**
  ******************************************************************************
  * @file    common.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    ���ļ�Ϊ�ڲ��ļ����û�������ú��޸�  
  ******************************************************************************
  */
#include "common.h"
#include <string.h>
#include "systick.h"


#define MCGOUT_TO_CORE_DIVIDER           (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT) + 1)
#define MCGOUT_TO_SYSTEM_DIVIDER         (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT) + 1)
#define MCGOUT_TO_BUS_DIVIDER            (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT) + 1)
#define MCGOUT_TO_PERIPHERAL_DIVIDER     (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT) + 1)
#define MCGOUT_TO_FLASH_DIVIDER          (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT) + 1)

 /**
 * @brief  ���ϵͳ��������ʱ�ӵ�Ƶ��
 * @code
 *         //�������ʱ��Ƶ��
 *         uint32_t BusClock;
 *         CLOCK_GetClockFrequency(kBusClock, &BusClock);
 *         //������ʱ��Ƶ����ʾ����
 *         printf("BusClock:%dHz\r\n", BusClock);
 * @endcode
 * @param  clockName:ʱ������
 *         @arg kCoreClock    :�ں�ʱ��
 *         @arg kSystemClock  :ϵͳʱ�� = �ں�ʱ��
 *         @arg kBusClock     :����ʱ��
 *         @arg kFlexBusClock :Flexbus����ʱ��
 *         @arg kFlashClock   :Flash����ʱ��
 * @param  FrequenctInHz: ���Ƶ�����ݵ�ָ�� ��λHz
 * @retval 0: �ɹ� ��0: ����
 */
int32_t CLOCK_GetClockFrequency(CLOCK_Source_Type clockName, uint32_t* FrequenctInHz)
{
    uint32_t MCGOutClock = 0;
    /* calualte MCGOutClock system_MKxxx.c must not modified */
    MCGOutClock = SystemCoreClock * MCGOUT_TO_CORE_DIVIDER;
    switch (clockName)
    {
        case kCoreClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_CORE_DIVIDER;
            break;
        case kSystemClock:
            *FrequenctInHz = MCGOutClock / MCGOUT_TO_SYSTEM_DIVIDER;
            break;	
        case kBusClock:
            *FrequenctInHz = SystemCoreClock / MCGOUT_TO_BUS_DIVIDER;
            break;
        case kFlashClock:
            *FrequenctInHz = SystemCoreClock / MCGOUT_TO_FLASH_DIVIDER;	
            break;
        case kMCGOutClock:
            *FrequenctInHz = MCGOutClock;
            break;
        default:
            return 1;
    }
    return 0;
}

 /**
 * @brief  ������ٳ�ʼ���ṹ �û��������
 *
 * @param  type: ���ٳ�ʼ���ṹ��ָ��
 * @retval       32λ���ٳ�ʼ������
 */
uint32_t QuickInitEncode(QuickInit_Type * type)
{
    return *(uint32_t*)type;
}

 /**
 * @brief  ������ٳ�ʼ���ṹ �û��������
 *
 * @param  map: 32λ���ٳ�ʼ������
 * @param  type: ���ٳ�ʼ���ṹָ��
 * @retval None
 */
void QuickInitDecode(uint32_t map, QuickInit_Type * type)
{
    QuickInit_Type * pMap = (QuickInit_Type*)&(map);
    memcpy(type, pMap, sizeof(QuickInit_Type));  
}

/**
 * @brief  ��ʱ��ʼ������
 * @code
 *      // �����ʱ��ʼ�����ã�
 *      //ʹ���ں˵�SYSticģ��ʵ����ʱ����
 *        DelayInit();
 * @endcode
 * @param  None
 * @retval None
 */
#pragma weak DelayInit
void DelayInit(void)
{
    SYSTICK_DelayInit();
}

/**
 * @brief ������뼶����ʱ���ú���
 * @code
 *      // ʵ��500ms����ʱ����
 *        DelayMs(500);
 * @endcode
 * @param  ms :��Ҫ��ʱ��ʱ�䣬��λ����
 * @retval None
 * @note  ������Ҫ�����ʱ��ʼ������
 */

#pragma weak DelayMs
void DelayMs(uint32_t ms)
{
    SYSTICK_DelayMs(ms);
}

/**
 * @brief ����΢�뼶����ʱ���ú���
 * @code
 *      // ʵ��500us����ʱ����
 *        DelayUs(500);
 * @endcode
 * @param  us :��Ҫ��ʱ��ʱ�䣬��λ΢��
 * @retval None
 * @note  ������Ҫ�����ʱ��ʼ������
 */
#pragma weak DelayUs
void DelayUs(uint32_t us)
{
    SYSTICK_DelayUs(us);
}



#if (defined(LIB_DEBUG))
void assert_failed(char * file, uint32_t line)
{
	//����ʧ�ܼ��
	while(1);
}
#endif

void NMI_Handler(void)
{
    
}

/*
    for(i=0;i<ARRAY_SIZE(I2C_QuickInitTable);i++)
    {
        printf("(0X%08XU)\r\n", QuickInitEncode(&I2C_QuickInitTable[i]));
    }
*/
