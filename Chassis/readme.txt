2022.10.8~10.9
1)解决了云台在陀螺仪模式下疯转问题：过零点处理
2)修改了bmi.c文件下Kp=0.4f
3)在rp_math.h中的宏定义限幅使用不了，添加了自己的限幅函数和过零点函数
4)新建了rp_user_define.h文件


2022.10.10
1)陀螺仪模式和机械模式切换之间，各自的目标值都要实时更新
	---在机械模式下一直去更改Z轴和Y轴的陀螺仪角度目标值
	---在陀螺仪模式下一直去更改Y轴电机的角度目标值
2)机械模式下云台比较抖
3)没有动遥控器下车会慢慢的偏移，更改修改了bmi.c文件下Kp=1
4)在rp_device_config.h中增添了dev_state_tran_t枚举类型，表示设备切换状态
  并在rc_sensor.h中rc_sensor_t添加了该类型，在rc_sensor.c中一开始对它赋值为DEV_OFF
  在RC_Heart_Task.c中添加了对遥控器开关机的状态变化的函数Judge_RC_Transtate和两个全局变量last_state,this_state
5)在gimbal.c中添加了Mechanical_Return函数，表示不管是什么模式，一开始让头回到机械位置，在RC_Heart_Task.c中进行调用
  当完成这个函数时，才能进入其他模式，并且这个函数完成后就不能再运行，也就是机械回位标志，在Chassis_Task中会用到
6)在gimbal.c文件中添加了GYRO_Mode函数，在rp_user_define.h中添加了Gryo_Mode的宏定义为1
7)在rp_math.h增加了限幅整型的函数ConstrainInt
8)在Imu_Task.h中对ggz做了一个处理，使得pitch发生剧烈变化后ggz数据正常
  因为pitch不等于0时，ggz不与底盘垂直
9)成功实现了遥控拨码拨到极限最大速度都为8000，在chassi.c中，并且在chassis_Task.c中要把麦伦速度结算放在最后  


2022.10.11
1)成功实现了小陀螺功能
2)将gimbal.c中有关speed_z的赋值操作搬运到了chassis.c文件中的Chassis_MEC_Input函数
3)在rp_user_define.h中增添了小陀螺模式下speed_z的赋值
4)增加了一个RC_Dial_Task.c的任务，优先级高于普通，专门对遥控器的拨码和对应状态标志位进行处理
5)在rp_user_define.h中把左拨码对应的模式英文改成小写
6)在chassis.c文件中会使用到RC_Dial_Task.c中的模式标志位
7)增加了云台最大角度8192转变为360°的比例和弧度转换比例----用于正余弦函数的使用
8)宏定义如果有计算，记得用括号，比如rp_user_define.h中的Mec_To_Angle_Rate
9)增加了另外一个版本的过零点函数---只修改误差值
10)在gimbal.c文件中多加了发送云台数组的函数，在Mechanical_Return中必须要调用，在Chassis_Task.c中也会调用
11)在rp_math.c中增添了两个绝对值函数，不会强行改变传入的数字正负，但是宏定义的绝对值函数会强行改变
12)修改了chassis.c文件中的麦伦速度解算对于speed[4]数组的限幅----因为一开始用的是宏定义的绝对值函数，误差变为负的极限值后会变为正
13)在rp_device_config.h中增添了枪管电机的数组索引号，在can_protocol.h中增添了枪管电机的ID
14)写完了枪管4个电机的收发数据


2022.10.12
1)成功实现了发射小弹丸（纪念今晚跟文轩和张硕的熬夜战---2点才走）
2)在rp_user_define.h中加入了低通滤波定义