#include "bsp_ws281x.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

uint8_t pixelBuffer[PIXEL_NUM][GRB];
/**
  * @brief  initialize WS281x LED
  * @param  None
  * @retval None
  */
void ws281x_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  WS281x_GPIO_CLK_FUNCTION(WS281x_GPIO_CLK, ENABLE);
  GPIO_InitStruct.GPIO_Pin = WS281x_GPIO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(WS281x_GPIOx, &GPIO_InitStruct);
  GPIO_ResetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
	
  
//  ws281x_closeAll();
//  delay_ms(100);
}

/**
  * @brief  ◊®”√”⁄WS281xµ∆÷È«˝∂Øµƒ—” ±∫Ø ˝
  * @param  count£∫—” ± ˝ £® æ≤®∆˜≤‚¡ø—” ± ±º‰ = delay_num * 440ns £©
  * @retval None
  */
static void ws281x_delay(uint32_t count)
{
  while(count--);
}

/**
  * @brief  ∑¢ÀÕ0¬Î£¨¥À¥¶∂‘ ±º‰“™«Û∫‹∏ﬂ£¨≤Ÿ◊˜IO∂Àø⁄÷ªƒ‹Õ®π˝ºƒ¥Ê∆˜∑Ω Ω≤Ÿ◊˜£¨
  * @param  None
  * @retval None
  */
void ws281x_sendTero(void)
{
  //GPIO_SetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
  GPIOA->BSRR = WS281x_GPIO_PIN; 
  ws281x_delay(1);
  //GPIO_ResetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
  GPIOA->BRR = WS281x_GPIO_PIN;
  ws281x_delay(2);
}

/**
  * @brief  ∑¢ÀÕ1¬Î£¨¥À¥¶∂‘ ±º‰“™«Û∫‹∏ﬂ£¨≤Ÿ◊˜IO∂Àø⁄÷ªƒ‹Õ®π˝ºƒ¥Ê∆˜∑Ω Ω≤Ÿ◊˜£¨
  * @param  None
  * @retval None
  */
void ws281x_sendOne(void)
{
  //GPIO_SetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
  GPIOA->BSRR = WS281x_GPIO_PIN;
  ws281x_delay(2);
  //GPIO_ResetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
  GPIOA->BRR = WS281x_GPIO_PIN;
  ws281x_delay(1);
}

/**
  * @brief  ∑¢ÀÕ∏¥Œª
  * @param  None
  * @retval None
  */
void ws281x_sendReset(void)
{
  GPIO_ResetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
  ws281x_delay(114);  //—” ±50us
  //GPIO_SetBits(WS281x_GPIOx, WS281x_GPIO_PIN);
}

/**
  * @brief  œ‘ æ∫Ø ˝£¨Ω´œ‘¥Ê ˝æ›œ‘ æµΩµ∆÷È…œ°£
  * @param  None
  * @retval None
  */
void ws281x_show(void)

{
		u16 i;
u8 j;
  for( i = 0; i < PIXEL_NUM; ++i)
  {
    for( j = 0; j < GRB; ++j)
    {
      if(pixelBuffer[i][j] == WS_HIGH)
      {
        ws281x_sendOne();
      }
      else
      {
        ws281x_sendTero();
      }
    }
  }
}

/**
  * @brief  close all led
  * @param  None
  * @retval None
  */
void ws281x_closeAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      pixelBuffer[i][j] = WS_LOW;
    }
  }
  ws281x_show(); 
}

/**
  * @brief  Ω´RGB—’…´◊È∫œµΩ“ª∏ˆ32Œª ˝æ›÷–¥Ê¥¢
  * @param  red£∫  0-255
  *         green£∫0-255
  *         blue£∫ 0-255
  * @retval None
  */
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

/**
  * @brief  ∏¯Ãÿ∂®LEDµ∆…Ë∂®—’…´
  * @param  £Ó£∫LEDµ∆∫≈
°°*         GRBClor: 32Œªµƒ—’…´÷µ
  * @retval None
  */
void ws281x_setPixelColor(uint16_t n, uint32_t GRBColor)
{
  uint8_t i;
	i=0;
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < GRB; i++)
    {
      pixelBuffer[n][i] = ((GRBColor << i) & 0x800000) ? WS_HIGH : WS_LOW;
    }
  }
}

void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint16_t i;
	i=0;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < GRB; i++)
    {
      pixelBuffer[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ws281x_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281x_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281x_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281x_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void ws281x_colorWipe(uint32_t c, uint8_t wait) {
	int i;
  for(i=-1; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    delay_ms(wait);
  }
}

void ws281x_rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM; i++) {
      ws281x_setPixelColor(i, ws281x_wheel((i+j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ws281x_rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM; i++) {
      ws281x_setPixelColor(i,ws281x_wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

//Theatre-style crawling lights.
void ws281x_theaterChase(uint32_t c, uint8_t wait) {
	int j;
		int q;
			u16 i;
  for (j=0; j<10; j++) {  //do 10 cycles of chasing
    for (q=0; q < 3; q++) {
      for (i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, c);    //turn every third pixel on
      }
      ws281x_show();
      delay_ms(wait);
      for (i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void ws281x_theaterChaseRainbow(uint8_t wait) {
	int j;
	int q;
	u16 i;
  for (j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (q=0; q < 3; q++) {
      for (i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, ws281x_wheel( (i+j) % 255));    //turn every third pixel on
      }
      ws281x_show();

      delay_ms(wait);
      for (i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void RGB_flag_color()
{
	
	extern char data[20];
	u8 flag_data_RGB;
	extern u8 USB_HUB_2_2;
	USB_HUB_2_2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
	
				flag_data_RGB = 0;
		if(strcmp(data,"#@f893")==0)
			
		{
			flag_data_RGB = 1; //µÁ≥ÿ°¢¬˙µÁ
		}
			else if(strcmp(data,"#@f8a3")==0)
			
		{
			flag_data_RGB = 2;	//µÁ≥ÿ°¢’˝≥£	
		}
			else if(strcmp(data,"#@f8C3")==0)
			
		{
			flag_data_RGB = 3;	//µÁ≥ÿ°¢«∑µÁ
		}
			else	if(strcmp(data,"#@f913")==0)
			
		{
			flag_data_RGB = 4; //AC°¢¬˙µÁ
		}	
		else	if(strcmp(data,"#@f923")==0)
			
		{
			flag_data_RGB = 5; //AC°¢’˝≥£
		}	
		else	if(strcmp(data,"#@f943")==0)
			
		{
			flag_data_RGB = 6; //AC°¢«∑µÁ
		}	
		
		
		{
		switch(flag_data_RGB)	
		{
							
			case 1:		ws281x_setPixelColor(1,0x000000);
				      ws281x_setPixelColor(0,0x0F0000);//µÁ≥ÿπ©µÁ
							ws281x_setPixelColor(2,0x0F0000);//¬˙µÁ¡ø
//								ws281x_show();
	break;
			case 2: ws281x_setPixelColor(1,0x000000);
								ws281x_setPixelColor(0,0x0F0000);//µÁ≥ÿπ©µÁ
								ws281x_setPixelColor(2,0x0F0F00);//’˝≥£µÁ¡ø
//								ws281x_show();
	break;
			case 3: ws281x_setPixelColor(1,0x000000);
								ws281x_setPixelColor(0,0x0F0000);//µÁ≥ÿπ©µÁ
								ws281x_setPixelColor(2,0x000F00);//µÕµÁ¡ø
//								ws281x_show();
	break;
		case 4: 		ws281x_setPixelColor(0,0x000000);
								ws281x_setPixelColor(1,0x0F0000);//ACπ©µÁ
								ws281x_setPixelColor(2,0x0F0000);//¬˙µÁ
//								ws281x_show();
	break;
		case 5: ws281x_setPixelColor(0,0x000000);
								ws281x_setPixelColor(1,0x0F0000);//ACπ©µÁ
								ws281x_setPixelColor(2,0x0F0F00);//’˝≥£µÁ¡ø
//								ws281x_show();
		break;
				case 6: ws281x_setPixelColor(0,0x000000);
								ws281x_setPixelColor(1,0x0F0000);//ACπ©µÁ
								ws281x_setPixelColor(2,0x000F00);//µÕµÁ¡ø
//								ws281x_show();
		break;
		}
//		ws281x_show();
		if(USB_HUB_2_2==0x01)
		{
			ws281x_setPixelColor(3,0x0F0000);
//			ws281x_show();
		}
	
		else if(USB_HUB_2_2==0x00)
		{
			ws281x_setPixelColor(3,0x0F0F00);
//			ws281x_show();
		}
	}
		ws281x_show();
}
//µÁ≥ÿ◊¥Ã¨µ∆
void rgbled_void()
{
	ws281x_setPixelColor(0,0x000000);
	ws281x_setPixelColor(1,0x000000);
	ws281x_setPixelColor(2,0x000000);
	ws281x_setPixelColor(3,0x000000);
	ws281x_setPixelColor(4,0x000000);
	ws281x_show();
}
//RGBµ∆∞Â≥ı ºªØ√µ
void GPS_rgbled()
{
	 extern u8 USB_HUB_2_2;
	if(USB_HUB_2_2==0x01)
		{
			ws281x_setPixelColor(3,0x0F0000);
			ws281x_show();
		}
	
		else if(USB_HUB_2_2==0x00)
		{
			ws281x_setPixelColor(3,0x0F0F00);
			ws281x_show();
		}
}
