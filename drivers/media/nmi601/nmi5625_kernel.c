#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <media/v4l2-int-device.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <mach/board.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <linux/regulator/consumer.h>
#include <mach/regulator.h>
#include <linux/miscdevice.h>
#include "nm5625_kernel.h"
#include "nmi_gpio_i2c.h"

/**************************************************************
	Global Defines & Variable
**************************************************************/

struct nmi_5625_dev {
	struct i2c_client *i2c_client_atv;

	struct mutex mu;
	struct class *tv_class;
	dev_t devn;
	struct	cdev cdv;
};

static int already_init = 0;
static struct nmi_5625_dev nd;

#if defined(CONFIG_MACH_SP7715GA_5701)
#define GPIO_ATV_RESET	  181
#define GPIO_ATV_PWROFF   183
#else
#define NMI_HW_I2C
#define NMI_VOLTAGE_USE_VDDCAMA

#define GPIO_ATV_RESET    64
#define GPIO_ATV_PWROFF   63
#endif

static struct regulator *s_cammotvdd_regulator = NULL;
static struct regulator *s_vddsim2_regulator = NULL;
#ifdef NMI_VOLTAGE_USE_VDDCAMA
static struct regulator *s_vddcama_regulator = NULL;
#endif


/**************************************************************
		file operation:
**************************************************************/

static int nmi5625_open(struct inode *inode, struct file *file)
{
	int ret = 0;

	func_enter();
	    dPrint(N_TRACE,"<<<<<<<<<<<<  nmi5625_open   >>>>>>>>>>>>\n");
	if (!already_init) {
	 dPrint(N_TRACE,"<<<<<<<<<<<<  nmi5625_open  fail already_init is not  already  >>>>>>>>>>>>\n");
		ret = -ENODEV;
		goto _fail_;
	}

	file->private_data = (void *)&nd;
 


_fail_:

	func_exit();
	return ret;
}

static int nmi5625_release(struct inode * inode, struct file * file)
{
	int ret = 0;

	func_enter();
	gpio_direction_output(GPIO_ATV_RESET,0);
        gpio_direction_output(GPIO_ATV_PWROFF,0);

	#ifndef NMI_HW_I2C
		nmi_i2c_deinit();
	#endif
	func_exit();
	return ret;
}

#define NMI_I2C_RW_LENGTH	256
static u8 i2cBuf[NMI_I2C_RW_LENGTH];

static int nmi5625_ioctl(struct file *file,
		    unsigned int cmd, unsigned long arg)
{
    struct nmi_5625_dev *d = file->private_data;
    int ret = 0;

    dPrint(N_TRACE,"[nmi-debug]%s: enter --> cmd = 0x%x\n", __func__,cmd&0xffff0000);

    switch ((cmd&0xffff0000)) {
        case NM5625_PWR_2P8_CTL:
#ifdef NMI_VOLTAGE_USE_VDDCAMA
			if (arg == 1) {	/* on */
				gpio_request(GPIO_MAIN_SENSOR_PWN, "main camera");
				gpio_direction_output(GPIO_MAIN_SENSOR_PWN, 1);
				gpio_free(GPIO_MAIN_SENSOR_PWN);

				gpio_request(GPIO_SUB_SENSOR_PWN, "main camera");
				gpio_direction_output(GPIO_SUB_SENSOR_PWN, 1);
				gpio_free(GPIO_SUB_SENSOR_PWN);

				if (NULL == s_cammotvdd_regulator) {
					s_cammotvdd_regulator = regulator_get(NULL, "vddcammot");
				}

				if (IS_ERR(s_cammotvdd_regulator)) {
					dPrint(N_ERR,"SENSOR:could not get vddcammot.\n");
					goto _fail_;
				}
				regulator_set_voltage( s_cammotvdd_regulator, 2800000, 2800000);
				regulator_enable( s_cammotvdd_regulator);
				regulator_put(s_cammotvdd_regulator);
				s_cammotvdd_regulator = NULL;
				
				if (NULL == s_vddcama_regulator) {
					s_vddcama_regulator = regulator_get(NULL, "vddcama");
				}

				if (IS_ERR(s_vddcama_regulator)) {
					dPrint(N_ERR,"SENSOR:could not get vddcama.\n");
					goto _fail_;
				}
				regulator_set_voltage( s_vddcama_regulator, 2800000, 2800000);
				regulator_enable( s_vddcama_regulator);
				regulator_put(s_vddcama_regulator);
				s_vddcama_regulator = NULL;
			} 
			else	{							
				gpio_request(GPIO_MAIN_SENSOR_PWN, "main camera");
				gpio_direction_output(GPIO_MAIN_SENSOR_PWN, 0);
				gpio_free(GPIO_MAIN_SENSOR_PWN);

				gpio_request(GPIO_SUB_SENSOR_PWN, "main camera");
				gpio_direction_output(GPIO_SUB_SENSOR_PWN, 0);
				gpio_free(GPIO_SUB_SENSOR_PWN);

				if (NULL == s_cammotvdd_regulator) {
					s_cammotvdd_regulator = regulator_get(NULL, "vddcammot");
				}

				if (IS_ERR(s_cammotvdd_regulator)) {
					dPrint(N_ERR,"SENSOR:could not get vddcammot.\n");
					goto _fail_;
				}
				regulator_disable( s_cammotvdd_regulator);
				regulator_put(s_cammotvdd_regulator);
				s_cammotvdd_regulator = NULL;
				
				if (NULL == s_vddcama_regulator) {
					s_vddcama_regulator = regulator_get(NULL, "vddcama");
				}

				if (IS_ERR(s_vddcama_regulator)) {
					dPrint(N_ERR,"SENSOR:could not get vddcama.\n");
					goto _fail_;
				}
				regulator_disable( s_vddcama_regulator);
				regulator_put(s_vddcama_regulator);
				s_vddcama_regulator = NULL;
			}
#else
            if (arg == 1) {	
                dPrint(N_TRACE,"set s_vddsim2_regulator voltage to level 1800mv\n");
      	       /* s_cammotvdd_regulator = regulator_get(NULL, "vddcammot");
               if (IS_ERR(s_cammotvdd_regulator)) {     
				s_cammotvdd_regulator = NULL;	
				printk(N_ERR,"SENSOR:could not get camiovdd.\n");
                break;
		        }*/
				regulator_set_voltage(s_cammotvdd_regulator,1800000,1800000);
	            regulator_enable(s_cammotvdd_regulator);

				//i2c  init
				#ifndef NMI_HW_I2C
				nmi_i2c_init();
				#endif
            }else{							
                regulator_disable(s_cammotvdd_regulator); 
               // regulator_put(s_cammotvdd_regulator);
               // s_cammotvdd_regulator = NULL;
            }
#endif
            break;
			
        case NM5625_PWR_1P2_CTL:
            dPrint(N_TRACE,"NM5625_PWR_1P2_CTL, power %s\n", (arg==1)?"on":"off");
#ifdef NMI_VOLTAGE_USE_VDDCAMA
			if (NULL == s_cammotvdd_regulator) {
				s_cammotvdd_regulator = regulator_get(NULL, "vddcammot");
			}

			if (IS_ERR(s_cammotvdd_regulator)) {
				dPrint(N_ERR,"SENSOR:could not get vddcammot.\n");
				goto _fail_;
			}
			
            if (arg == 1) {	
                gpio_direction_output(GPIO_ATV_PWROFF,1);
                //ANA_REG_OR(ANA_MIXED_CTRL,  BIT_6);
                regulator_set_voltage(s_cammotvdd_regulator,2800000,2800000);
                regulator_enable(s_cammotvdd_regulator);
            } 
            else {
                /* sun.aijun(SPRD) modify */
                gpio_direction_output(GPIO_ATV_PWROFF,0);
                //ANA_REG_AND(ANA_MIXED_CTRL,  ~BIT_6);
                regulator_disable(s_cammotvdd_regulator);   
            }	
			regulator_put(s_cammotvdd_regulator);
			s_cammotvdd_regulator = NULL;
#else
            if (arg == 1) {	
                gpio_direction_output(GPIO_ATV_PWROFF,1);
                //ANA_REG_OR(ANA_MIXED_CTRL,  BIT_6);
               /* dPrint(N_TRACE,"set s_cammotvdd_regulator voltage to level 1800mv\n");
                regulator_set_voltage(s_cammotvdd_regulator,1800000,1800000);
                regulator_enable(s_cammotvdd_regulator);*/
            } 
            else {
                /* sun.aijun(SPRD) modify */
                gpio_direction_output(GPIO_ATV_PWROFF,0);
                //ANA_REG_AND(ANA_MIXED_CTRL,  ~BIT_6);
                //regulator_disable(s_cammotvdd_regulator);   
            }	
#endif
            break;

        case NM5625_ATV_RESET_CTL:
            dPrint(N_TRACE,"NM5625_ATV_RESET_CTL, reset %s\n", (arg==1)?"high":"low");
            if (arg == 1) {						
                gpio_direction_output(GPIO_ATV_RESET,1);
            } 
            else {						
                gpio_direction_output(GPIO_ATV_RESET,0);
            }
            break;


        case NM5625_ATV_I2C_READ:
            {
                u8 *kbuf = &i2cBuf[0];
                int size = cmd&0xffff;	/* Note: I used the lower 16 bits for size */	
                int len = size;
                //dPrint(N_TRACE,"NM5625_ATV_I2C_READ\n");
                mutex_lock(&d->mu);
		#ifdef NMI_HW_I2C
                while(len) {
                    int sz;
                    if (len > NMI_I2C_RW_LENGTH)
                        sz = NMI_I2C_RW_LENGTH;
                    else
                        sz = len;
                    ret = i2c_master_recv(d->i2c_client_atv, kbuf, sz); 
                    if (ret < 0) {
                        dPrint(N_ERR, "nmi: failed i2c read...(%d)\n", ret);
                        //kfree(kbuf);
                        mutex_unlock(&d->mu);
                        goto _fail_;
                    }
                    kbuf += NMI_I2C_RW_LENGTH;
                    len -= sz;
                }

                if (copy_to_user(arg, i2cBuf, size) ) {
                    dPrint(N_ERR, "nmi: failed copy to user...\n");
                    ret = -EFAULT;
                    //kfree(kbuf);
                    mutex_unlock(&d->mu);
                    goto _fail_;
                }
		#else
				ret = nmi_i2c_read(0x60,kbuf,len);
				
				//dPrint(N_TRACE,"kernel:nmi_i2c_read buf is (%x), length is (%d)\n",kbuf,len);

				if (copy_to_user(arg, i2cBuf, len) ) {
					dPrint(N_ERR, "nmi: failed copy to user...\n");
					ret = -EFAULT;
					mutex_unlock(&d->mu);
					goto _fail_;
				}
			
		#endif
                //kfree(kbuf);
                mutex_unlock(&d->mu);
            }
            break;
        case NM5625_ATV_I2C_WRITE:
            {
                u8 *kbuf = &i2cBuf[0];
                int size = cmd&0xffff;	/* Note: I used the lower 16 bits for size */
                int len = size;
                //dPrint(N_TRACE,"NM5625_ATV_I2C_WRITE\n");

                if (copy_from_user(kbuf, arg, size)) {					
                    dPrint(N_ERR, "nmi: failed copy from user...\n");
                    ret = -EFAULT;
                    goto _fail_;
                }
                //dPrint(N_TRACE,"nmi: write buf is (%x), size is (%d)\n",*kbuf,size);
                mutex_lock(&d->mu);
		#ifdef NMI_HW_I2C
                while(len){
                    int sz;
                    if (len > NMI_I2C_RW_LENGTH)
                        sz = NMI_I2C_RW_LENGTH;
                    else
                        sz = len;
                    ret = i2c_master_send(d->i2c_client_atv, kbuf, sz);
                    if (ret < 0) {
                        dPrint(N_ERR, "nmi: failed i2c write...(%d)\n", ret);
                        //kfree(kbuf);
                        mutex_unlock(&d->mu);
                        goto _fail_;
                    }

                    kbuf += NMI_I2C_RW_LENGTH;
                    len -= sz;
                }

		#else
				if (copy_from_user(kbuf, arg, len)) {					
					dPrint(N_ERR, "nmi: failed copy from user...\n");
					ret = -EFAULT;
					goto _fail_;
				}
					
				ret = nmi_i2c_write(0x60,kbuf,len);
				dPrint(N_TRACE,"kernel:nmi_i2c_write buf is (%x), length is (%d)\n",kbuf,len);
		#endif

                //nmi_i2c_write(0x60,kbuf,size);
                //dPrint(N_TRACE,"nmi: write buf is (%x), size is (%d)\n",*kbuf,size);

                mutex_unlock(&d->mu);
            }
            break;
        default:
            break;
    }

_fail_:
    //func_exit();
    //dPrint(N_TRACE, "nmi_ioctl return value...(%d)\n", ret);
    return ret; 
}




static const struct file_operations nmi5625_fops = {
	.owner		= THIS_MODULE,
 	.unlocked_ioctl			= nmi5625_ioctl,
	.open		= nmi5625_open,
	.release	= nmi5625_release,
};

/**************************************************************
	
	i2c:

**************************************************************/

static int nmi5625_remove(struct i2c_client *client)
{
	int ret = 0;

	func_enter();

	nd.i2c_client_atv = NULL;
	func_exit();
	
	return ret;
}

static int nmi5625_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, "nmiatv");                                                         
    return 0;                                                                                       
}        

static int nmi5625_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;
	struct device *dev;
	func_enter();

	if (!already_init) {
		memset(&nd, 0, sizeof(struct nmi_5625_dev));
		mutex_init(&nd.mu);

		if ((ret = alloc_chrdev_region(&nd.devn, 0, 1, "nmi")) < 0) {
			dPrint(N_ERR, "nmi: failed unable to get major...%d\n", ret);
			goto _fail_;
		}
		dPrint(N_INFO, "nmi:dynamic major(%d),minor(%d)\n", MAJOR(nd.devn), MINOR(nd.devn));

		cdev_init(&nd.cdv, &nmi5625_fops);
		nd.cdv.owner = THIS_MODULE;
		ret = cdev_add(&nd.cdv, nd.devn, 1);
		if (ret) {
			dPrint(N_ERR, "nmi: failed to add device...%d\n", ret);
			goto _fail_;
		}

		nd.tv_class = class_create(THIS_MODULE, "atv");
		if (IS_ERR(nd.tv_class)) {
			dPrint(N_ERR, "nmi: failed to create the atv class\n");
		}

		dev = device_create(nd.tv_class, NULL, nd.devn, NULL, "nmi");
		if (IS_ERR(dev)) {
			dPrint(N_ERR, "nmi: failed to create device\n");
		}

/*    if (NULL == s_vddsim2_regulator) {
    if (NULL == s_vddsim2_regulator) {
        s_vddsim2_regulator = regulator_get(NULL, "vddsim2");
        regulator_set_voltage(s_vddsim2_regulator,1800000,1800000);
        regulator_enable(s_vddsim2_regulator);
    }
    if (IS_ERR(s_vddsim2_regulator)) {
        dPrint(N_ERR,"SENSOR:could not get vddsim2.\n");
    }
    
    s_cammotvdd_regulator = regulator_get(NULL, "vddcamio");
    }
}
    if (IS_ERR(s_cammotvdd_regulator)) {
    	 regulator_set_voltage(s_cammotvdd_regulator,1800000,1800000);
        //regulator_enable(s_cammotvdd_regulator);
        dPrint(N_ERR,"SENSOR:could not get camiovdd.\n");
    }*/

#ifdef NMI_VOLTAGE_USE_VDDCAMA
		s_vddcama_regulator = regulator_get(NULL, "vddcama");

        if (IS_ERR(s_vddcama_regulator)) {     
		printk(N_ERR,"SENSOR:could not get camiovdd.\n");
	    }
		
		regulator_put(s_vddcama_regulator);
		s_vddcama_regulator = NULL;
#else
		s_cammotvdd_regulator = regulator_get(NULL, "vddcammot");

        if (IS_ERR(s_cammotvdd_regulator)) {     
		s_cammotvdd_regulator = NULL;	
		printk(N_ERR,"SENSOR:could not get camiovdd.\n");
	    }
#endif

		already_init = 1;
	}
	
	nd.i2c_client_atv = client;
		
_fail_:
	func_exit();
	return ret;
}

static const struct i2c_device_id nmi5625_id[] = {
	{"nmiatv", 0},
	{},
};

static struct i2c_driver nmi5625_i2c_driver = {
	.driver = {
		  .owner = THIS_MODULE,
		  .name  = "nmiatv",
		  },
	.probe  = nmi5625_probe,
    .detect = nmi5625_detect,
	.remove = nmi5625_remove,
	.id_table = nmi5625_id,
};

/**************************************************************
	
	Module:

**************************************************************/
static int __init nmi5625_module_init(void)
{
	int ret=-1;

	func_enter();

 	/***************************************
	initialize 2.8V 1.2V RESET GPIO mode  for reference
 	 ****************************************/	
	gpio_request(GPIO_ATV_RESET,"atv_reset");
	gpio_request(GPIO_ATV_PWROFF,"atv_pwr_off");

	gpio_direction_output(GPIO_ATV_RESET,0);
        gpio_direction_output(GPIO_ATV_PWROFF,0);

	ret = i2c_add_driver(&nmi5625_i2c_driver);
	if (ret < 0) {
		dPrint(N_ERR, "nmi: failed register i2c driver...(%d)\n", ret);
	}

	func_exit();

	return ret;
}

static void __exit nmi5625_module_clean(void)
{
	func_enter();

	i2c_del_driver(&nmi5625_i2c_driver);
	gpio_free(GPIO_ATV_RESET);
	gpio_free(GPIO_ATV_PWROFF);
        regulator_put(s_cammotvdd_regulator);
        s_cammotvdd_regulator = NULL;

	if (already_init) {
		device_destroy(nd.tv_class, nd.devn);
		cdev_del(&nd.cdv);
		unregister_chrdev_region(nd.devn, 1);
		already_init = 0;
	}

	func_exit();
}

module_init(nmi5625_module_init);
module_exit(nmi5625_module_clean);


MODULE_AUTHOR("qzhu");
MODULE_DESCRIPTION("nmi TV 5625 driver");
MODULE_LICENSE("GPL");
