#include <stdio.h>
#include "receive_agreement.h"
#include <stdlib.h>
#include <string.h>
#define _head_1 0x23        //֡ͷ'#'
#define _head_2 0x40        //֡ͷ'@'
PUartHandle pUartHandle;
void Init_data_array(char *data)     //�����ʼ��
{
    int i = 0;
    while (data[i++] != '\0')
    {
        data[i] = 0;
    }
}

void pUartHandle_init(void) //�ṹ���ʼ������    
{

    pUartHandle.step = 0;

    pUartHandle.tmpCnt = 0;

    pUartHandle.aRxBufIndex = 0;
}
/*
���պ���
��һ�������ǽ��ܵ����ַ����ڶ���Ϊ��������
����������Ҫ�û��Զ���
�Һ������ڽ����ж��������У���֤ʵʱ��
*/
 void Receive_Data(uint8_t str, char *Data)
{  
     
    switch (pUartHandle.step)
    {
    case 0:
      
        if (str == _head_1)   //֡ͷ����
        {

            pUartHandle.step++;
            pUartHandle.aRxBuf_1[pUartHandle.aRxBufIndex++] = str; /*?????*/
        }
        break;
    case 1:
        if (str == _head_2)  //�ڶ���֡ͷ����
        {
            pUartHandle.step++;
            pUartHandle.aRxBuf_1[pUartHandle.aRxBufIndex++] = str;
           
        }
        else if (str == _head_1) //��ֹ֡ͷ�������ظ�
            pUartHandle.step = 1;
        else
        {
            
            pUartHandle_init();
        }

        break;

    case 2:
        if (str == _head_1)
        {
            pUartHandle.step = 1;
            pUartHandle.aRxBufIndex = 1;
        }
        else
        {
           
            pUartHandle.tmpCnt =(int)str; //�����ݳ��ȸ�ֵ��TMPCNT
            pUartHandle.step++;
            pUartHandle.aRxBuf_1[pUartHandle.aRxBufIndex++] = str;                           /*��ʼ��������*/
            if (((RX_BUF_1_LENGTH - pUartHandle.aRxBufIndex) < str) || (str == 0x00)) //��ֹ������������յ�����ֵ
            {

                pUartHandle_init();
            }
        }
        break;

    case 3:
      
        pUartHandle.aRxBuf_1[pUartHandle.aRxBufIndex++] = str;
        pUartHandle.tmpCnt--;
        if (pUartHandle.aRxBufIndex >= RX_BUF_1_LENGTH)
        {
            pUartHandle_init();
        }
        if (pUartHandle.tmpCnt == 0) //˵���������
        {
            strcpy(Data, pUartHandle.aRxBuf_1);
            Init_data_array(pUartHandle.aRxBuf_1);  //ÿ����һ֡���ݺ󻺳������㡣��ֹ���
            pUartHandle_init();                       //��ʼ��
        }

        break;

    default:
        pUartHandle_init();
        break;
    }
}
