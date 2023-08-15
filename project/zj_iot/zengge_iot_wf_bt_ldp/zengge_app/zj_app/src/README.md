
 ## void light_color_covert(color_convert_t *para)通用颜色设置接口使用说明


### 静态颜色设置(建议方法1)
#### note : 执行类型为EXECUTION_HSV时，自动会切换模式类型为静态模式类型。
- 方法1
   
```
如设置红色：
convert_data.execution_type  = EXECUTION_HSV;
convert_data.h               = 0; 
convert_data.s               = 100;
convert_data.rgb_v           = 100;

convert_data.transition_time = 30;
convert_data.delay_time      = 0;
light_color_covert(&convert_data);

```
 - 方法2
   
```
如设置红色：
convert_data.execution_type  = EXECUTION_RGB;
convert_data.red             = 0xFF; 
convert_data.green           = 0;
convert_data.blue            = 0;

convert_data.transition_time = 30;
convert_data.delay_time      = 0;
light_color_covert(&convert_data);

```

### 动态模式中颜色设置
#### note : 需搭配模式类型设置light_set_mode_type()和亮度设置light_set_rgb_lightness()使用。
```
如添加一个红色频闪到场景模式类型中：

红色频闪5次： 
void red_blink()
{

	for(int i = 0;i < 5;i++){

		convert_data.execution_type  = EXECUTION_RGBWC_DYNAMIC;
		convert_data.red             = 0xFF; 
		convert_data.green           = 0;
		convert_data.blue            = 0;

		convert_data.transition_time = 0;
		convert_data.delay_time      = 0;
		light_color_covert(&convert_data);

		延时100ms;
		convert_data.red             = 0; 
		convert_data.green           = 0;
		convert_data.blue            = 0;

		convert_data.transition_time = 0;
		convert_data.delay_time      = 0;
		light_color_covert(&convert_data);
		延时500ms;
	}
}

red_blink()放到new_scene_mode_handler()中执行，假如running_mode = 4：
void new_scene_mode_handler()
{
      if(light_get_mode_type() == MODE_TYPE_FIX_SCENE){

      switch(light_get_running_mode()){

         case 1:
            __scene_grassland();
         break;
		 case 2:
		 
		 break;
		 .....
		 case 4:
		    red_blink()
		 break;
	  }
}

外部调用方法：

light_set_running_mode(4); //运行模式4
light_set_speed(80); //速度值80
light_set_rgb_lightness(50,0); //亮度50
light_set_mode_type(MODE_TYPE_FIX_SCENE);//场景模式类型

```