#ifndef _CDC_COM_H_
#define _CDC_COM_H_

int tp_device_id(int id);
void ctp_lock_mutex(void);
void ctp_unlock_mutex(void);

int CDC_Gsensor_Device_Id(int id);

int CDC_Plsensor_Device_Id(int id);

#endif
