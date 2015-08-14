#include "spi.h"
#include "delay.h"
void SPI_init(int flag)//�ı��ٶ�
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//���˿�ʱ�ӡ�SPIʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	//���˿�ʱ�ӡ�SPIʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5����Ϊ SPI1
		//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1
	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPIΪ˫��˫��ȫ˫��ģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//���͡�����8λ֡�ṹ
  SPI_InitStructure.SPI_CPOL =SPI_CPOL_High ; //ʼ�����ո�  // SPI_CPOL_Low//ʼ�����յ� 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//��2��ʱ���ز��� //SPI_CPHA_1Edge��1��ʱ���ز��� 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	 //Ӳ������NSS�źţ�ss�� �ó����ʱ,NSS�ſ�������	  // SPI_NSS_Hard 
  if(flag==2)  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //Ԥ��ƵֵΪ2
  else if(flag==4) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //Ԥ��ƵֵΪ4
  else if(flag==8) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //Ԥ��ƵֵΪ8
  else if(flag==16) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  //Ԥ��ƵֵΪ16
  else if(flag==32) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//Ԥ��ƵֵΪ32
  else if(flag==64) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //Ԥ��ƵֵΪ64
  else if(flag==128) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;  //Ԥ��ƵֵΪ128
  else if(flag==256) SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //Ԥ��ƵֵΪ256
  else  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //Ԥ��ƵֵΪ4 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ��������λ��ʼ	   //SD����λ�ȴ���
  SPI_InitStructure.SPI_CRCPolynomial = 7;	 //������CRCֵ����Ķ���ʽΪ7
  SPI_Init(SPI1, &SPI_InitStructure); 
  SPI_Cmd(SPI1,ENABLE); 
}
char SPI_SendReceive(char data)     //SPI1���շ�
  {		  
  	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  //while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
      SPI_I2S_SendData(SPI1, data);	
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	  return  SPI_I2S_ReceiveData(SPI1);

  } 
