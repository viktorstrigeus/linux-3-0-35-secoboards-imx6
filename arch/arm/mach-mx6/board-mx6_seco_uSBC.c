/*
 * Copyright (C) 2011-2013 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/version.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/nodemask.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/i2c.h>
#include <linux/i2c/pca953x.h>
#include <linux/ata.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/regulator/consumer.h>
#include <linux/pmic_external.h>
#include <linux/pmic_status.h>
#include <linux/ipu.h>
#include <linux/mxcfb.h>
#include <linux/pwm_backlight.h>
#include <linux/fec.h>
#include <linux/memblock.h>
#include <linux/gpio.h>
#include <linux/etherdevice.h>
#include <linux/regulator/anatop-regulator.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/i2c/tsc2004.h>
#include <linux/spi/tsc2006.h>
#include <linux/i2c/econtroller.h>
#include <linux/rtc-bridge.h>

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/mxc_dvfs.h>
#include <mach/memory.h>
#include <mach/iomux-mx6q.h>
#include <mach/imx-uart.h>
#include <mach/viv_gpu.h>
#include <mach/ahci_sata.h>
#include <mach/ipu-v3.h>
#include <mach/mxc_hdmi.h>
#include <mach/mxc_asrc.h>
#include <mach/board.h>

#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include "usb.h"
#include "devices-imx6q.h"
#include "crm_regs.h"
#include "cpu_op-mx6.h"

#include "board-mx6qd_seco_uSBC.h"
#include "board-mx6sdl_seco_uSBC.h"

/******************* SD *******************/
//#define MX6_SECO_uSBC_SD1_CD			IMX_GPIO_NR(6, 15)
//#define MX6_SECO_uSBC_SD1_WP			IMX_GPIO_NR(6, 14)
//#define MX6_SECO_uSBC_SD4_CD			IMX_GPIO_NR(2, 6)
//#define MX6_SECO_uSBC_SD4_WP			IMX_GPIO_NR(5, 20)
/******************* SATA *******************/
#define MX6_SECO_uSBC_SATA_LED		IMX_GPIO_NR(6, 11)
/******************* USB *******************/
#define MX6_SECO_uSBC_USB_OTG_PWR	IMX_GPIO_NR(3, 22)
#define MX6_SECO_uSBC_USB_HUB_RESET	IMX_GPIO_NR(7, 12)
/******************* PMIC *******************/
#ifdef CONFIG_Q7_PMIC_PFUZE100
#define MX6_SECO_uSBC_PMIC_INT		IMX_GPIO_NR(5, 16)
#endif
/******************* LVDS *******************/
#define MX6_SECO_uSBC_LVDS_BLT_CTRL   IMX_GPIO_NR(1, 4)
#define MX6_SECO_uSBC_LVDS_PPEN_CTRL  IMX_GPIO_NR(1, 2)
/******************* ETHERNET *******************/
#define MX6_SECO_uSBC_FEC_RESET		IMX_GPIO_NR(3, 23)
#define MX6_ENET_125MHz_EN			IMX_GPIO_NR(6, 24)
/******************* SPI *******************/
/******************* REV A ***************/
#define MX6_SECO_uSBC_ECSPI1_CS1		IMX_GPIO_NR(3, 19)
/******************* REV B *****************/
#define MX6_SECO_uSBC_CSPI1_CS1      IMX_GPIO_NR(3, 19)
#define MX6_SECO_uSBC_CSPI3_CS3      IMX_GPIO_NR(4, 24)
/******************* ST1232 TOUCHSCREEN *******************/
#define MX6_SECO_ST1232_TOUCH_RST	IMX_GPIO_NR(3, 26)	
#define MX6_SECO_ST1232_TOUCH_INT	IMX_GPIO_NR(3, 27)	
/******************* AUDIO *******************/
#if defined(CONFIG_Q7_SND_SOC_IMX_AC97_VT1613) || defined(CONFIG_Q7_SND_SOC_IMX_AC97_GENERIC)
#define AC97_GPIO_TXFS				IMX_GPIO_NR(1, 11)
#define SSI_CH_NUMBER				1
#endif
/******************* CAN *******************/
#define MX6_SECO_uSBC_CAN_S     	IMX_GPIO_NR(1, 0)
#define MX6_SECO_uSBC_CAN_TERM  	IMX_GPIO_NR(1, 1)

#define MX6_SECO_uSBC_CAP_TCH_INT1	IMX_GPIO_NR(1, 9)
/******************* CAMERA CSI *******************/
#define MX6_CAMERA_RST			IMX_GPIO_NR(6, 5)
#define MX6_CAMERA_POWER_DOWN		IMX_GPIO_NR(6, 4)
/******************* EMBEDDED CONTROLLER *******************/
#define MX6_SECO_USBC_ECTRL_IRQ         IMX_GPIO_NR(2, 7)

#define MX6_SECO_uSBC_SD3_WP_PADCFG	(PAD_CTL_PKE | PAD_CTL_PUE |	\
		PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED |	\
		PAD_CTL_DSE_40ohm | PAD_CTL_HYS)


/*
#define MX6_SECO_Q7_CAP_TCH_INT1	IMX_GPIO_NR(1, 9)
#define MX6_SECO_Q7_CAN1_STBY		IMX_GPIO_NR(1, 2)
#define MX6_SECO_Q7_CAN1_EN			IMX_GPIO_NR(1, 4)

#define MX6_SECO_Q7_CSI0_RST		IMX_GPIO_NR(4, 22)
#define MX6_SECO_Q7_CSI0_PWN		IMX_GPIO_NR(4, 21)


#define MX6_SECO_Q7_SATA_LED		IMX_GPIO_NR(6, 11)

#define MX6_SECO_Q7_HDMI_CHECK		IMX_GPIO_NR(5, 2)
#define MX6_SECO_Q7_FEC_RESET		IMX_GPIO_NR(3, 23)

#define MX6_SECO_Q7_IO_MICRO       IMX_GPIO_NR(2, 4)
*/
#define MX6_SECO_Q7_SD3_WP_PADCFG	(PAD_CTL_PKE | PAD_CTL_PUE |	\
		PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED |	\
		PAD_CTL_DSE_40ohm | PAD_CTL_HYS)

void __init early_console_setup(unsigned long base, struct clk *clk);
static struct clk *sata_clk;

extern char *gp_reg_id;
extern char *soc_reg_id;
extern char *pu_reg_id;


extern struct regulator *(*get_cpu_regulator)(void);
extern void (*put_cpu_regulator)(void);


/***********************************************************************
 *                                   SD                                *
 ***********************************************************************/

/* for QUAD and DUAL */
static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 50);
static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 100);
static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 200);

/* for SOLO and DUAL LITE */
static iomux_v3_cfg_t MX6DL_USDHC_PAD_SETTING(3, 50);
static iomux_v3_cfg_t MX6DL_USDHC_PAD_SETTING(3, 100);
static iomux_v3_cfg_t MX6DL_USDHC_PAD_SETTING(3, 200);

enum sd_pad_mode {
	SD_PAD_MODE_LOW_SPEED,
	SD_PAD_MODE_MED_SPEED,
	SD_PAD_MODE_HIGH_SPEED,
};

static int plt_sd3_pad_change(int clock) {
	static enum sd_pad_mode pad_mode = SD_PAD_MODE_LOW_SPEED;

	if (clock > 100000000) {
		if (pad_mode == SD_PAD_MODE_HIGH_SPEED)
			return 0;

		pad_mode = SD_PAD_MODE_HIGH_SPEED;
		if (cpu_is_mx6q()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6q_sd3_200mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		} else if (cpu_is_mx6dl()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6dl_sd3_200mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		}
	} else if (clock > 52000000) {
		if (pad_mode == SD_PAD_MODE_MED_SPEED)
			return 0;

		pad_mode = SD_PAD_MODE_MED_SPEED;
		if (cpu_is_mx6q()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6q_sd3_100mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		} else if (cpu_is_mx6dl()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6dl_sd3_100mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		}
	} else {
		if (pad_mode == SD_PAD_MODE_LOW_SPEED)
			return 0;

		pad_mode = SD_PAD_MODE_LOW_SPEED;
		if (cpu_is_mx6q()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6q_sd3_50mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		} else if (cpu_is_mx6dl()) {
			return mxc_iomux_v3_setup_multiple_pads(mx6dl_sd3_50mhz,
						ARRAY_SIZE(mx6dl_sd3_200mhz));
		}
	}
}


	/* MMC 2 - eMMC */
static const struct esdhc_platform_data mx6q_seco_q7_sd3_data __initconst = {
	.always_present = 1,
	.keep_power_at_suspend = 1,
	.platform_pad_change = plt_sd3_pad_change,
	.support_8bit = 1,
};



/***********************************************************************
 *                                  SATA                               *
 ***********************************************************************/

/* HW Initialization, if return 0, initialization is successful. */
static int mx6q_seco_q7_sata_init(struct device *dev, void __iomem *addr) {
	u32 tmpdata;
	int ret = 0;
	struct clk *clk;

	sata_clk = clk_get(dev, "imx_sata_clk");
	if (IS_ERR(sata_clk)) {
		dev_err(dev, "no sata clock.\n");
		return PTR_ERR(sata_clk);
	}
	ret = clk_enable(sata_clk);
	if (ret) {
		dev_err(dev, "can't enable sata clock.\n");
		goto put_sata_clk;
	}

	/* Set PHY Paremeters, two steps to configure the GPR13,
	 * one write for rest of parameters, mask of first write is 0x07FFFFFD,
	 * and the other one write for setting the mpll_clk_off_b
	 *.rx_eq_val_0(iomuxc_gpr13[26:24]),
	 *.los_lvl(iomuxc_gpr13[23:19]),
	 *.rx_dpll_mode_0(iomuxc_gpr13[18:16]),
	 *.sata_speed(iomuxc_gpr13[15]),
	 *.mpll_ss_en(iomuxc_gpr13[14]),
	 *.tx_atten_0(iomuxc_gpr13[13:11]),
	 *.tx_boost_0(iomuxc_gpr13[10:7]),
	 *.tx_lvl(iomuxc_gpr13[6:2]),
	 *.mpll_ck_off(iomuxc_gpr13[1]),
	 *.tx_edgerate_0(iomuxc_gpr13[0]),
	 */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x07FFFFFF) | 0x0593A044), IOMUXC_GPR13);

	/* enable SATA_PHY PLL */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x2) | 0x2), IOMUXC_GPR13);

	/* Get the AHB clock rate, and configure the TIMER1MS reg later */
	clk = clk_get(NULL, "ahb");
	if (IS_ERR(clk)) {
		dev_err(dev, "no ahb clock.\n");
		ret = PTR_ERR(clk);
		goto release_sata_clk;
	}
	tmpdata = clk_get_rate(clk) / 1000;
	clk_put(clk);

#ifdef CONFIG_Q7_SATA_AHCI
	ret = sata_init(addr, tmpdata);
	if (ret == 0)
		return ret;
#else
	usleep_range(1000, 2000);
	/* AHCI PHY enter into PDDQ mode if the AHCI module is not enabled */
	tmpdata = readl(addr + PORT_PHY_CTL);
	writel(tmpdata | PORT_PHY_CTL_PDDQ_LOC, addr + PORT_PHY_CTL);
	pr_info("No AHCI save PWR: PDDQ %s\n", ((readl(addr + PORT_PHY_CTL)
					>> 20) & 1) ? "enabled" : "disabled");
#endif

release_sata_clk:
	clk_disable(sata_clk);
put_sata_clk:
	clk_put(sata_clk);

	return ret;
}

#ifdef CONFIG_Q7_SATA_AHCI
static void mx6q_seco_q7_sata_exit(struct device *dev) {
	clk_disable(sata_clk);
	clk_put(sata_clk);
}

static struct ahci_platform_data mx6q_seco_q7_sata_data = {
	.init = mx6q_seco_q7_sata_init,
	.exit = mx6q_seco_q7_sata_exit,
};

#endif



/***********************************************************************
 *                                  UART                               *
 ***********************************************************************/


static inline void mx6q_seco_uSBC_init_uart(void) {
	imx6q_add_imx_uart(1, NULL);
	imx6q_add_imx_uart(4, NULL);    // 4 wire UART
}



/***********************************************************************
 *                                   USB                               *
 ***********************************************************************/

static void imx6q_seco_q7_usbotg_vbus(bool on) {
	if (on)
		gpio_set_value(MX6_SECO_uSBC_USB_OTG_PWR, 1);
	else
		gpio_set_value(MX6_SECO_uSBC_USB_OTG_PWR, 0);
}

static void __init imx6q_seco_q7_init_usb(void) {
	int ret = 0;

	imx_otg_base = MX6_IO_ADDRESS(MX6Q_USB_OTG_BASE_ADDR);
	/* disable external charger detect,
	 * or it will affect signal quality at dp .
	 */
	ret = gpio_request(MX6_SECO_uSBC_USB_OTG_PWR, "usb-pwr");
	if (ret) {
		pr_err("failed to get GPIO MX6_SECO_uSBC_USB_OTG_PWR: %d\n",
			ret);
		return;
	}
	gpio_direction_output(MX6_SECO_uSBC_USB_OTG_PWR, 0);
	mxc_iomux_set_gpr_register(1, 13, 1, 0);

	mx6_set_otghost_vbus_func(imx6q_seco_q7_usbotg_vbus);
//	mx6_usb_dr_init();
//	mx6_usb_h1_init();
}



/***********************************************************************
 *                               GPU - IPU                             *
 ***********************************************************************/

static struct viv_gpu_platform_data imx6q_gpu_pdata __initdata = {
	.reserved_mem_size = SZ_128M,
};

static struct imx_asrc_platform_data imx_asrc_data = {
	.channel_bits = 4,
	.clk_map_ver = 2,
};

static struct imx_ipuv3_platform_data ipu_data[] = {
	{
	.rev = 4,
	.csi_clk[0] = "clko2_clk",
	}, {
	.rev = 4,
	.csi_clk[0] = "clko2_clk",
	},
};



/***********************************************************************
 *                               ETHERNET                              *
 ***********************************************************************/

static int mx6q_seco_q7_fec_phy_reset(struct phy_device *phydev) {
int ret;
        if(cpu_is_mx6q())
                mxc_iomux_v3_setup_pad(MX6Q_PAD_RGMII_RX_CTL__GPIO_6_24);
        if(cpu_is_mx6dl())
                mxc_iomux_v3_setup_pad(MX6DL_PAD_RGMII_RX_CTL__GPIO_6_24);
        ret = gpio_request(MX6_ENET_125MHz_EN, "125mHz_en");
        if (!ret) {
                gpio_direction_output(MX6_ENET_125MHz_EN, 1);
                gpio_set_value(MX6_ENET_125MHz_EN, 1);
                printk("Resetting ethernet physical layer.\n");
                gpio_set_value(MX6_SECO_uSBC_FEC_RESET, 0);
                msleep(2);
                gpio_set_value(MX6_SECO_uSBC_FEC_RESET, 1);
                msleep(1);
                gpio_free(MX6_ENET_125MHz_EN);
                if(cpu_is_mx6q())
                        mxc_iomux_v3_setup_pad(MX6Q_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL);
                if(cpu_is_mx6dl())
                        mxc_iomux_v3_setup_pad(MX6DL_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL);
        } else {
                printk(KERN_ERR "Reset of ethernet physical layer failed.\n");
        }
        return 0;
}

static int mx6q_seco_q7_fec_phy_init(struct phy_device *phydev) {
	int prod_ID;
	/* prefer master mode, disable 1000 Base-T capable */
	//phy_write(phydev, 0x9, 0x1c00);
	prod_ID = phy_read(phydev, 0x3);

#define KSZ9021_id 0x1611
#define KSZ9031_id_1 0x1621
#define KSZ9031_id_2 0x1622

        switch (prod_ID) {
                case KSZ9021_id:
			printk("Activating ethernet physical layer Micrel KSZ9021 Gigabit PHY.\n");
			/* min rx data delay */
			phy_write(phydev, 0x0b, 0x8105);
			phy_write(phydev, 0x0c, 0x0000);

			/* max rx/tx clock delay, min rx/tx control delay */
			phy_write(phydev, 0x0b, 0x8104);
			phy_write(phydev, 0x0c, 0xf0f0);
//			phy_write(phydev, 0x0b, 0x104);
			break;

                case KSZ9031_id_1:
            printk("Activating ethernet physical layer Micrel KSZ9031 Gigabit PHY.\n");
			phy_write(phydev, 0x0d, 0x0002); // reg. select operation
			phy_write(phydev, 0x0e, 0x0008); // reg. idx address
			phy_write(phydev, 0x0d, 0x4002); // data operation - no autoinc.
			phy_write(phydev, 0x0e, 0x03FF);
			break;
                case KSZ9031_id_2:
            printk("Activating ethernet physical layer Micrel KSZ9031 Gigabit PHY.\n");
                        phy_write(phydev, 0x0d, 0x0002); // reg. select operation
                        phy_write(phydev, 0x0e, 0x0008); // reg. idx address
                        phy_write(phydev, 0x0d, 0x4002); // data operation - no autoinc.
                        phy_write(phydev, 0x0e, 0x03FF);
                        break;

                default:
                        printk("Warning: Product ID of physical layer not recognized [id = 0x%04x unknown]\n", prod_ID);
                        break;
        }

	return 0;
}

static struct fec_platform_data fec_data __initdata = {
	.init = mx6q_seco_q7_fec_phy_init,
//	.reset = mx6q_seco_q7_fec_phy_reset,
	.phy = PHY_INTERFACE_MODE_RGMII,
};



/***********************************************************************
 *                                  HDMI                               *
 ***********************************************************************/

static void hdmi_init(int ipu_id, int disp_id) {
	int hdmi_mux_setting;

	if ((ipu_id > 1) || (ipu_id < 0)) {
		pr_err("Invalid IPU select for HDMI: %d. Set to 0\n", ipu_id);
		ipu_id = 0;
	}

	if ((disp_id > 1) || (disp_id < 0)) {
		pr_err("Invalid DI select for HDMI: %d. Set to 0\n", disp_id);
		disp_id = 0;
	}

	/* Configure the connection between IPU1/2 and HDMI */
	hdmi_mux_setting = 2*ipu_id + disp_id;

	/* GPR3, bits 2-3 = HDMI_MUX_CTL */
	mxc_iomux_set_gpr_register(3, 2, 2, hdmi_mux_setting);

	mxc_iomux_set_gpr_register(0, 0, 1, 1);
}

static struct fsl_mxc_hdmi_platform_data hdmi_data = {
	.init = hdmi_init,
};

static struct fsl_mxc_hdmi_core_platform_data hdmi_core_data = {
	.ipu_id = 0,
	.disp_id = 0,
};



/***********************************************************************
 *                                 VIDEO                               *
 ***********************************************************************/

static struct ipuv3_fb_platform_data seco_q7_fb_data[] = {
	{ /*fb0*/
	.disp_dev = "ldb",
	.interface_pix_fmt = IPU_PIX_FMT_RGB24,
	.mode_str = "LDB-WSXGA",
	.default_bpp = 16,
	.int_clk = false,
	}, {
	.disp_dev = "ldb",
	.interface_pix_fmt = IPU_PIX_FMT_RGB666,
	.mode_str = "CLAA-WVGA",
	.default_bpp = 16,
	.int_clk = false,
	}, {
	.disp_dev = "ldb",
	.interface_pix_fmt = IPU_PIX_FMT_RGB666,
	.mode_str = "CLAA-WVGA",
	.default_bpp = 16,
	.int_clk = false,
	},
};

static struct fsl_mxc_lcd_platform_data lcdif_data = {
	.ipu_id = 0,
	.disp_id = 0,
	.default_ifmt = IPU_PIX_FMT_RGB565,
};

static struct fsl_mxc_ldb_platform_data ldb_data = {
	.ipu_id = 1,
	.disp_id = 0,
	.ext_ref = 1,
	.mode = LDB_SEP0,
	.sec_ipu_id = 1,
	.sec_disp_id = 1,
};

static struct platform_pwm_backlight_data mx6_seco_q7_pwm_backlight_data = {
	.pwm_id = 0,
	.max_brightness = 255,
	.dft_brightness = 128,
	.pwm_period_ns = 50000,
};

static struct fsl_mxc_capture_platform_data capture_data[] = {
	{
		.csi = 0,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 0,
	}, {
		.csi = 1,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 1,
	},
};



/***********************************************************************
 *                                  AUDIO                              *
 ***********************************************************************/

#if defined(CONFIG_Q7_SND_SOC_IMX_AC97_VT1613) || defined(CONFIG_Q7_SND_SOC_IMX_AC97_GENERIC)

static struct mxc_audio_platform_data mx6_seco_q7_audio_data;

static void audio_codec_ac97_cold_reset(struct snd_ac97 *ac97) {

 int ret1, ret2, ret, reset;

	reset = AC97_GPIO_TXFS;
        
        ret1 = gpio_request(reset, "ac97_reset");
        if (ret1) {
                printk (KERN_ERR "request MX6_AUD_RST failed!");
                return;
        }

        if(cpu_is_mx6dl()){
                mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN2__GPIO_4_18);
                mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN3__GPIO_4_19);
        }
        if(cpu_is_mx6q()){
                mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN2__GPIO_4_18);
                mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN3__GPIO_4_19);
        }
        ret1 = gpio_request(IMX_GPIO_NR(4,18), "ac97_strap1");
        ret2 = gpio_request(IMX_GPIO_NR(4,19), "ac97_strap2");
        if (ret || ret1 || ret2) {
                printk (KERN_ERR "request MX6_AUD_RST ac97_strap1 ac97_strap2 failed!");
        } else {
			gpio_direction_output(reset, 0);
			gpio_direction_output(IMX_GPIO_NR(4,18), 0);
			udelay(2);
			gpio_set_value(reset, 1);
			gpio_free(IMX_GPIO_NR(4,18));
			gpio_free(IMX_GPIO_NR(4,19));

//                gpio_direction_output(reset, 1);
//                gpio_direction_output(IMX_GPIO_NR(4,18), 0);
//                gpio_direction_output(IMX_GPIO_NR(4,19), 0);
//                udelay(2);
//                gpio_set_value(reset, 0);
//                mdelay(1);
//                gpio_set_value(reset, 1);
//                mdelay(1);
//                gpio_free(IMX_GPIO_NR(4,18));
//                gpio_free(IMX_GPIO_NR(4,19));
        }
        if(cpu_is_mx6dl()){
                mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN2__AUDMUX_AUD6_TXD);
                mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN3__AUDMUX_AUD6_TXFS);
		mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN4__AUDMUX_AUD6_RXD);
                mxc_iomux_v3_setup_pad(MX6DL_PAD_DI0_PIN15__AUDMUX_AUD6_TXC);
        }
        if(cpu_is_mx6q()){
                mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN2__AUDMUX_AUD6_TXD);
                mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN3__AUDMUX_AUD6_TXFS);
		mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN4__AUDMUX_AUD6_RXD);
                mxc_iomux_v3_setup_pad(MX6Q_PAD_DI0_PIN15__AUDMUX_AUD6_TXC);
        }
        printk("audio_codec_ac97_cold_reset \n");

}

static struct imx_ssi_platform_data mx6_seco_q7_ssi_pdata = {
    .ac97_reset             = audio_codec_ac97_cold_reset,
	.flags = (IMX_SSI_USE_AC97 | IMX_SSI_DMA),
};

static struct mxc_audio_platform_data mx6_seco_q7_audio_data = {
	.ssi_num = SSI_CH_NUMBER,
	.src_port = SSI_CH_NUMBER,
	.ext_port = 6, // OUTPUT ON AUD6 FOR AUDIO MUXING
	.hp_gpio = -1,
};
#endif
#if defined(CONFIG_Q7_SND_SOC_IMX_AC97_VT1613)
struct vt1613_codec_vibra_data {
        unsigned int    vibra_mclk;
};

struct vt1613_codec_audio_data {
        unsigned int    audio_mclk;
};

struct vt1613_codec_data {
        unsigned int    audio_mclk;
        struct vt1613_codec_audio_data          *audio;
        struct vt1613_codec_vibra_data          *vibra;
};

static struct platform_device mx6_seco_q7_imx_audio_device = {
        .name           = "imx-vt1613",
};

static struct platform_device mx6_seco_q7_audio_device = {
        .name           = "vt1613-ac97",
};
#endif
#if defined(CONFIG_Q7_SND_SOC_IMX_AC97_GENERIC)
static struct platform_device mx6_seco_q7_imx_audio_device = {
        .name           = "imx-generic",
};

static struct platform_device mx6_seco_q7_audio_device = {
        .name           = "generic-ac97",
};
#endif

static int imx6q_init_audio(void) {
#if defined(CONFIG_Q7_SND_SOC_IMX_AC97_VT1613) || defined(CONFIG_Q7_SND_SOC_IMX_AC97_GENERIC)
	mxc_register_device(&mx6_seco_q7_imx_audio_device, &mx6_seco_q7_audio_data);
	mxc_register_device(&mx6_seco_q7_audio_device, NULL);
	imx6q_add_imx_ssi(SSI_CH_NUMBER - 1, &mx6_seco_q7_ssi_pdata);
#endif
	return 0;
}



/***********************************************************************
 *                            POWER MANAGEMENT                         *
 ***********************************************************************/

static const struct anatop_thermal_platform_data
	mx6q_seco_q7_anatop_thermal_data __initconst = {
		.name = "anatop_thermal",
};

static struct mxc_dvfs_platform_data seco_q7_dvfscore_data = {
	.reg_id = "cpu_vddgp",
	.soc_id = "cpu_vddsoc",
	.pu_id = "cpu_vddvpu",
	.clk1_id = "cpu_clk",
	.clk2_id = "gpc_dvfs_clk",
	.gpc_cntr_offset = MXC_GPC_CNTR_OFFSET,
	.ccm_cdcr_offset = MXC_CCM_CDCR_OFFSET,
	.ccm_cacrr_offset = MXC_CCM_CACRR_OFFSET,
	.ccm_cdhipr_offset = MXC_CCM_CDHIPR_OFFSET,
	.prediv_mask = 0x1F800,
	.prediv_offset = 11,
	.prediv_val = 3,
	.div3ck_mask = 0xE0000000,
	.div3ck_offset = 29,
	.div3ck_val = 2,
	.emac_val = 0x08,
	.upthr_val = 25,
	.dnthr_val = 9,
	.pncthr_val = 33,
	.upcnt_val = 10,
	.dncnt_val = 10,
	.delay_time = 80,
};

static void seco_q7_suspend_enter(void) {
	/* suspend preparation */
}

static void seco_q7_suspend_exit(void) {
	/* resume restore */
}

static const struct pm_platform_data mx6q_seco_q7_pm_data __initconst = {
	.name = "imx_pm",
	.suspend_enter = seco_q7_suspend_enter,
	.suspend_exit = seco_q7_suspend_exit,
};

static struct regulator_consumer_supply seco_q7_vmmc_consumers[] = {
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.1"),
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.2"),
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.3"),
};

static struct regulator_init_data seco_q7_vmmc_init = {
	.num_consumer_supplies = ARRAY_SIZE(seco_q7_vmmc_consumers),
	.consumer_supplies = seco_q7_vmmc_consumers,
};

static struct fixed_voltage_config seco_q7_vmmc_reg_config = {
	.supply_name	= "vmmc",
	.microvolts		= 3300000,
	.gpio			= -1,
	.init_data		= &seco_q7_vmmc_init,
};

static struct platform_device seco_q7_vmmc_reg_devices = {
	.name	= "reg-fixed-voltage",
	.id	= 3,
	.dev	= {
		.platform_data = &seco_q7_vmmc_reg_config,
	},
};



/***********************************************************************
 *                                  PCIE                               *
 ***********************************************************************/

#if defined(CONFIG_Q7_PCIE) 
static const struct imx_pcie_platform_data mx6_seco_q7_pcie_data  __initconst = {
	.pcie_pwr_en	= -EINVAL,
	.pcie_rst	= -EINVAL,
	.pcie_wake_up	= -EINVAL,
	.pcie_dis	= -EINVAL,
};
#endif

/***********************************************************************
 *                    MIPI - CSI 5Mpx OV5640 CAMERA                    *
 ***********************************************************************/

#if (defined(CONFIG_MXC_CAMERA_OV5640_MIPI) || defined(CONFIG_MXC_CAMERA_OV5640_MIPI_MODULE)) && (defined(CONFIG_Q7_CAM))

static void ov5640_mipi_camera_io_init(void) {
        struct clk *clko;


        gpio_request(MX6_CAMERA_POWER_DOWN, "cam-power-down");
        gpio_direction_output(MX6_CAMERA_POWER_DOWN, 0);

        clko = clk_get(NULL, "clko2_clk");
        if (IS_ERR(clko)) {
                pr_err("can't get CLKO2 clock.\n");
        } else {
                long round = clk_round_rate(clko, 27000000);
                clk_set_rate(clko, round);
                clk_enable(clko);
        }

        /* Camera reset */
        gpio_request(MX6_CAMERA_RST, "cam-reset");
        gpio_direction_output(MX6_CAMERA_RST, 0);
        msleep(1);
        gpio_set_value(MX6_CAMERA_RST, 1);
        msleep(100);

/* for mx6dl, mipi virtual channel 1 connect to csi 1*/
        if (cpu_is_mx6dl())
                mxc_iomux_set_gpr_register(13, 0, 3, 0);
}

static void ov5640_mipi_camera_powerdown(int powerdown) {
}

static struct fsl_mxc_camera_platform_data ov5640_mipi_data = {
        .mclk = 22000000,
        .csi = 0,
        .io_init = ov5640_mipi_camera_io_init,
        .pwdn = ov5640_mipi_camera_powerdown,
};
#endif //  #if (defined(CONFIG_MXC_CAMERA_OV5640_MIPI) || defined(CONFIG_MXC_CAMERA_OV5640_MIPI_MODULE)) && (defined(CONFIG_Q7_CAM))


/***********************************************************************
 *                                 SPI-NOR                             *
 ***********************************************************************/

#if defined(CONFIG_Q7_MTD_M25P80) 
static struct mtd_partition imx6_seco_q7_spi_nor_partitions[] = {
	{
	 .name = "bootloader",
	 .offset = 0,
	 .size = 0x00080000,
	},
	{
	 .name = "kernel",
	 .offset = MTDPART_OFS_APPEND,
	 .size = MTDPART_SIZ_FULL,
	},
};

static struct flash_platform_data imx6_seco_q7_spi_flash_data = {
	.name = "m25p80",
	.parts = imx6_seco_q7_spi_nor_partitions,
	.nr_parts = ARRAY_SIZE(imx6_seco_q7_spi_nor_partitions),
	.type = "sst25vf016b",
};
#endif



/***********************************************************************
 *                                   SPI                               *
 ***********************************************************************/
// REV A
static int mx6q_seco_q7_spi_cs_revA[] = {
	MX6_SECO_uSBC_ECSPI1_CS1,
};
// REV B
static int mx6q_seco_q7_spi_1_cs_revB[] = {
    	MX6_SECO_uSBC_CSPI1_CS1,
};
// REV C
static int mx6q_seco_q7_spi_3_cs_revB[] = {
        MX6_SECO_uSBC_CSPI3_CS3,
};

static int mx6q_seco_q7_spi_1_cs_revC[] = {
        MX6_SECO_uSBC_CSPI1_CS1,
};

static int mx6q_seco_q7_spi_3_cs_revC[] = {
        MX6_SECO_uSBC_CSPI3_CS3,
};
//REV A
static const struct spi_imx_master mx6q_seco_q7_spi_data_revA __initconst = {
	.chipselect     = mx6q_seco_q7_spi_cs_revA,
	.num_chipselect = ARRAY_SIZE(mx6q_seco_q7_spi_cs_revA),
};
//REV B
static const struct spi_imx_master mx6q_seco_q7_spi_1_data_revB __initconst = {
        .chipselect     = mx6q_seco_q7_spi_1_cs_revB,
        .num_chipselect = ARRAY_SIZE(mx6q_seco_q7_spi_1_cs_revB),
};

static const struct spi_imx_master mx6q_seco_q7_spi_3_data_revB __initconst = {
        .chipselect     = mx6q_seco_q7_spi_3_cs_revB,
        .num_chipselect = ARRAY_SIZE(mx6q_seco_q7_spi_3_cs_revB),
};
//REV C
static const struct spi_imx_master mx6q_seco_q7_spi_1_data_revC __initconst = {
        .chipselect     = mx6q_seco_q7_spi_1_cs_revC,
        .num_chipselect = ARRAY_SIZE(mx6q_seco_q7_spi_1_cs_revC),
};

static const struct spi_imx_master mx6q_seco_q7_spi_3_data_revC __initconst = {
        .chipselect     = mx6q_seco_q7_spi_3_cs_revC,
        .num_chipselect = ARRAY_SIZE(mx6q_seco_q7_spi_3_cs_revC),
};

static struct spi_board_info imx6_seco_q7_spi_device_revA[] __initdata = {
#if defined(CONFIG_Q7_MTD_M25P80)
	{
		.modalias = "m25p80",
		.max_speed_hz = 20000000, /* max spi clock (SCK) speed in HZ */
		.bus_num = 0,
		.chip_select = 0,
		.platform_data = &imx6_seco_q7_spi_flash_data,
	},
#endif
};


static struct spi_board_info imx6_seco_q7_spi_device_revB[] __initdata = {
#if defined(CONFIG_Q7_MTD_M25P80)
        {
                .modalias = "m25p80",
                .max_speed_hz = 20000000, /* max spi clock (SCK) speed in HZ */
                .bus_num = 0,
                .chip_select = 0,
                .platform_data = &imx6_seco_q7_spi_flash_data,
        },
#endif
#ifdef CONFIG_RTC_DRV_PCF2123
        {
              .modalias               = "rtc-pcf2123",
              .chip_select            = 0,
              .bus_num                = 2,
              //.controller_data        = (void *)AT91_PIN_PA10,
              .max_speed_hz           = 1000 * 1000,
              .mode                   = SPI_MODE_0 | SPI_CS_HIGH,
        },
#endif
};

static struct spi_board_info imx6_seco_q7_spi_device_revC[] __initdata = {
#if defined(CONFIG_Q7_MTD_M25P80)
        {
                .modalias = "m25p80",
                .max_speed_hz = 20000000, /* max spi clock (SCK) speed in HZ */
                .bus_num = 0,
                .chip_select = 0,
                .platform_data = &imx6_seco_q7_spi_flash_data,
        },
#endif
#ifdef CONFIG_RTC_DRV_PCF2123
        {
              .modalias               = "rtc-pcf2123",
              .chip_select            = 0,
              .bus_num                = 2,
              //.controller_data        = (void *)AT91_PIN_PA10,
              .max_speed_hz           = 1000 * 1000,
              .mode                   = SPI_MODE_0 | SPI_CS_HIGH,
        },
#endif
};

static void spi_device_init(void) {
	
	if ( PCB_IS_REV_A )
		spi_register_board_info(imx6_seco_q7_spi_device_revA,
		    ARRAY_SIZE(imx6_seco_q7_spi_device_revA));

	if ( PCB_IS_REV_B )
		spi_register_board_info(imx6_seco_q7_spi_device_revB,
                    ARRAY_SIZE(imx6_seco_q7_spi_device_revB));
	
	if ( PCB_IS_REV_C )
                spi_register_board_info(imx6_seco_q7_spi_device_revC,
                    ARRAY_SIZE(imx6_seco_q7_spi_device_revC));	
}



/***********************************************************************
 *                                   I2C                               *
 ***********************************************************************/

static struct imxi2c_platform_data mx6q_seco_q7_i2c0_data = {
	.bitrate = 100000,
};

static struct imxi2c_platform_data mx6q_seco_q7_i2c1_data = {
	.bitrate = 400000,
};

static struct imxi2c_platform_data mx6q_seco_q7_i2c2_data = {
	.bitrate = 400000,
};

static void pre_halt_signal (void) {
}

static void post_halt_signal (void) {
}

#ifdef CONFIG_Q7_ECONTROLLER
static enum ECTRL_EVENTS ectrl_event_list [6] __initdata = {
	EVNT_PWR_BTN,
	EVNT_RST_BTN,
	EVNT_SLEEP,
        EVNT_BATTERY,
	EVNT_LID,
	EVNT_WAKE,
};

static struct ectrl_bootdev bootdev_list [2] __initdata = {
	{
		.id     = BOOT_EMMC,
		.label  = "internal eMMC",
	}, {
		.id     = BOOT_SPI,
		.label  = "internal SPI flash",
	},
};

static struct ectrl_platform_data mx6_seco_q7_ectrl = {
	.task_pre_halt_signal   = pre_halt_signal,
	.task_post_halt_signal  = post_halt_signal,
	.event_list		= &ectrl_event_list,
	.nr_event               = ARRAY_SIZE (ectrl_event_list),
	.available_bootdev      = &bootdev_list,
	.nr_bootdev             = ARRAY_SIZE (bootdev_list),
	.irq			= gpio_to_irq(MX6_SECO_USBC_ECTRL_IRQ),
	.irq_flags		= IRQF_SHARED | IRQF_TRIGGER_FALLING,

};
#endif
#ifdef CONFIG_TOUCHSCREEN_SITRONIX_ST1232
static void sitronix_st1232_reset_ic(void) {

        printk("%s\n", __FUNCTION__);

	gpio_request(MX6_SECO_ST1232_TOUCH_RST,"reset-st1232");
        gpio_direction_output(MX6_SECO_ST1232_TOUCH_RST, 1);
        gpio_set_value(MX6_SECO_ST1232_TOUCH_RST, 0);
        mdelay(1);
        gpio_set_value(MX6_SECO_ST1232_TOUCH_RST, 1);

}
#endif
static struct i2c_board_info mxc_i2c0_board_info[] __initdata = {
#ifdef CONFIG_Q7_ECONTROLLER
	{
		I2C_BOARD_INFO("econtroller", 0x40),
       .platform_data 	= &mx6_seco_q7_ectrl,
    },
#endif
};

#ifdef CONFIG_Q7_RTC_BRIDGE
extern struct rtc_dev_share *dev_share;

static void rtc_bridge_share_init (void) {

        dev_share = kmalloc(sizeof(struct rtc_dev_share),GFP_KERNEL);
        dev_share->dev_internal = NULL;
        dev_share->dev_external = NULL;

}
#endif

static struct i2c_board_info mxc_i2c1_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("mxc_hdmi_i2c", 0x50),
	},
};
static struct i2c_board_info mxc_i2c2_board_info[] __initdata = {
#if (defined(CONFIG_MXC_CAMERA_OV5640_MIPI) || defined(CONFIG_MXC_CAMERA_OV5640_MIPI_MODULE)) && (defined(CONFIG_Q7_CAM))
        {
                I2C_BOARD_INFO("ov5640_mipi", 0x3c),
                .platform_data = (void *)&ov5640_mipi_data,
        },
#endif
#ifdef CONFIG_TOUCHSCREEN_SITRONIX_ST1232
	{
                I2C_BOARD_INFO("st1232-ts", 0x55),
		.irq            = gpio_to_irq(MX6_SECO_ST1232_TOUCH_INT),
 //               .platform_data  = &st1232_touch_i2c_conf,
        },
#endif
};



/***********************************************************************
 *                                   CAN                               *
 ***********************************************************************/

static const struct flexcan_platform_data imx6_seco_uSBC_flexcan0_pdata __initconst = {
        .transceiver_switch = NULL,
};


/***********************************************************************
 *                                   MIPI                              *
 ***********************************************************************/

static struct mipi_csi2_platform_data mipi_csi2_pdata = {
	.ipu_id	 = 0,
	.csi_id = 0,
	.v_channel = 0,
	.lanes = 2,
	.dphy_clk = "mipi_pllref_clk",
	.pixel_clk = "emi_clk",
};



/***********************************************************************
 *                               BOARD INIT                            *
 ***********************************************************************/

static void __init fixup_mxc_board(struct machine_desc *desc, struct tag *tags,
				   char **cmdline, struct meminfo *mi)
{
	PRINT_VERSION;
}

/*!
 * Board specific initialization.
 */
static void __init mx6_seco_q7_board_init(void)
{
	int i, ret;
	int can_silent, can_terminal;
	struct clk *clko2;
	struct clk *new_parent;
	int rate;
	
if (PCB_IS_REV_A)
                printk (KERN_INFO "Board Rev A\n");
    	else if (PCB_IS_REV_B)
                printk (KERN_INFO "Board Rev B\n");	
	else if (PCB_IS_REV_C)
		printk (KERN_INFO "Board Rev C\n");
	else
		printk (KERN_INFO "Unknown Board Revision\n");
		
	
	if (cpu_is_mx6q()){
		mxc_iomux_v3_setup_multiple_pads(mx6qd_seco_uSBC_pads,
			ARRAY_SIZE(mx6qd_seco_uSBC_pads));
		if (PCB_IS_REV_A)	
		mxc_iomux_v3_setup_multiple_pads(mx6qd_seco_uSBC_pads_revA,
			ARRAY_SIZE(mx6qd_seco_uSBC_pads_revA));
		if (PCB_IS_REV_B)
		mxc_iomux_v3_setup_multiple_pads(mx6qd_seco_uSBC_pads_revB,
                        ARRAY_SIZE(mx6qd_seco_uSBC_pads_revB));
		if (PCB_IS_REV_C)
                mxc_iomux_v3_setup_multiple_pads(mx6qd_seco_uSBC_pads_revC,
                        ARRAY_SIZE(mx6qd_seco_uSBC_pads_revC));
	}
	else if (cpu_is_mx6dl()) {
		mxc_iomux_v3_setup_multiple_pads(mx6sdl_seco_uSBC_pads,
			ARRAY_SIZE(mx6sdl_seco_uSBC_pads));

		if (PCB_IS_REV_A)       
                mxc_iomux_v3_setup_multiple_pads(mx6sdl_seco_uSBC_pads_revA,
                        ARRAY_SIZE(mx6sdl_seco_uSBC_pads_revA));
                if (PCB_IS_REV_B)
                mxc_iomux_v3_setup_multiple_pads(mx6sdl_seco_uSBC_pads_revB,
                        ARRAY_SIZE(mx6sdl_seco_uSBC_pads_revB));
		if (PCB_IS_REV_C)
		mxc_iomux_v3_setup_multiple_pads(mx6sdl_seco_uSBC_pads_revC,
                        ARRAY_SIZE(mx6sdl_seco_uSBC_pads_revC));
		
	}
/*	
	ret = gpio_request (MX6_SECO_USBC_ECTRL_IRQ, "IO MICRO");
	if (ret) {
		printk("failed to get MX6_SECO_USBC_ECTRL_IRQ: %d\n", ret);
	} else {	
		gpio_direction_input (MX6_SECO_USBC_ECTRL_IRQ);
	}
*/
#ifdef CONFIG_TOUCHSCREEN_SITRONIX_ST1232
	/* Resetting touch screen controller */
	sitronix_st1232_reset_ic();
#endif
	
	ret = gpio_request (MX6_SECO_uSBC_LVDS_BLT_CTRL, "LVDS backlight");
	if (ret) {
		printk("failed to get MX6_SECO_uSBC_LVDS_BLT_CTRL: %d\n", ret);
	} else {
		gpio_direction_output(MX6_SECO_uSBC_LVDS_BLT_CTRL, 1);
	}
	
	ret = gpio_request (MX6_SECO_uSBC_LVDS_PPEN_CTRL, "PPEN backlight");
        if (ret) {
                printk("failed to get MX6_SECO_uSBC_LVDS_PPEN_CTRL: %d\n", ret);
        } else {
                gpio_direction_output(MX6_SECO_uSBC_LVDS_PPEN_CTRL, 1);
        }
#ifdef CONFIG_Q7_RTC_BRIDGE
        //if (PCB_IS_REV_D)
                rtc_bridge_share_init();
#endif
#ifdef CONFIG_FEC_1588
	/* Set GPIO_16 input for IEEE-1588 ts_clk and RMII reference clock
	 * For MX6 GPR1 bit21 meaning:
	 * Bit21:       0 - GPIO_16 pad output
	 *              1 - GPIO_16 pad input
	 */
	mxc_iomux_set_gpr_register(1, 21, 1, 1);
#endif


//   GPIO for Ethernet reset
	gpio_request(MX6_SECO_uSBC_FEC_RESET, "fec-reset");
	gpio_direction_output(MX6_SECO_uSBC_FEC_RESET, 1);
//   GPIO for Ethernet reset
 
	gp_reg_id = seco_q7_dvfscore_data.reg_id;
	soc_reg_id = seco_q7_dvfscore_data.soc_id;
	pu_reg_id = seco_q7_dvfscore_data.pu_id;
	mx6q_seco_uSBC_init_uart();

	/*
	 * MX6DL/Solo only supports single IPU
	 * The following codes are used to change ipu id
	 * and display id information for MX6DL/Solo. Then
	 * register 1 IPU device and up to 2 displays for
	 * MX6DL/Solo
	 */
	if (cpu_is_mx6dl()) {
		ldb_data.ipu_id = 0;
		ldb_data.disp_id = 0;
		ldb_data.sec_ipu_id = 0;
		ldb_data.sec_disp_id = 1;
		hdmi_core_data.disp_id = 1;
	}

	imx6q_add_mxc_hdmi_core(&hdmi_core_data);

	imx6q_add_ipuv3(0, &ipu_data[0]);
	if (cpu_is_mx6q()) {
		imx6q_add_ipuv3(1, &ipu_data[1]);
		for (i = 0; i < 4 && i < ARRAY_SIZE(seco_q7_fb_data); i++)
			imx6q_add_ipuv3fb(i, &seco_q7_fb_data[i]);
	} else
		for (i = 0; i < 2 && i < ARRAY_SIZE(seco_q7_fb_data); i++)
			imx6q_add_ipuv3fb(i, &seco_q7_fb_data[i]);

	imx6q_add_vdoa();
	imx6q_add_lcdif(&lcdif_data);
	imx6q_add_ldb(&ldb_data);
	imx6q_add_v4l2_output(0);
	imx6q_add_v4l2_capture(0, &capture_data[0]);
	imx6q_add_v4l2_capture(1, &capture_data[1]);
#if (defined(CONFIG_MXC_CAMERA_OV5640_MIPI) || defined(CONFIG_MXC_CAMERA_OV5640_MIPI_MODULE)) && (defined(CONFIG_Q7_CAM))
        imx6q_add_mipi_csi2(&mipi_csi2_pdata);
#endif

#ifdef CONFIG_Q7_RTC_BRIDGE
        //if (PCB_IS_REV_D)
        imx6q_add_imx_rtc_bridge();
#endif
	imx6q_add_imx_snvs_rtc();

	imx6q_add_imx_i2c(0, &mx6q_seco_q7_i2c0_data);
	imx6q_add_imx_i2c(1, &mx6q_seco_q7_i2c1_data);
	imx6q_add_imx_i2c(2, &mx6q_seco_q7_i2c2_data);
	i2c_register_board_info(0, mxc_i2c0_board_info,
			ARRAY_SIZE(mxc_i2c0_board_info));
	i2c_register_board_info(1, mxc_i2c1_board_info,
			ARRAY_SIZE(mxc_i2c1_board_info));
#if (defined(CONFIG_MXC_CAMERA_OV5640_MIPI) || defined(CONFIG_MXC_CAMERA_OV5640_MIPI_MODULE)) && (defined(CONFIG_Q7_CAM)) || defined(CONFIG_TOUCHSCREEN_SITRONIX_ST1232)
	i2c_register_board_info(2, mxc_i2c2_board_info,
                        ARRAY_SIZE(mxc_i2c2_board_info));
#endif
		
#ifdef CONFIG_Q7_PMIC_PFUZE100
	
	ret = gpio_request(MX6_SECO_uSBC_PMIC_INT, "pFUZE-int");
	if (ret) {
		printk(KERN_ERR "request pFUZE-int error!!\n");
		return;
	} else {	
		gpio_direction_input(MX6_SECO_uSBC_PMIC_INT);
		mx6q_seco_q7_init_pfuze100(MX6_SECO_uSBC_PMIC_INT);
	}
	
#endif

	/* SPI */
	if ( PCB_IS_REV_A)
		imx6q_add_ecspi(0, &mx6q_seco_q7_spi_data_revA);
	if ( PCB_IS_REV_B){
                imx6q_add_ecspi(0, &mx6q_seco_q7_spi_1_data_revB);
		imx6q_add_ecspi(2, &mx6q_seco_q7_spi_3_data_revB);
	}
	if ( PCB_IS_REV_C){
		imx6q_add_ecspi(0, &mx6q_seco_q7_spi_1_data_revC);
                imx6q_add_ecspi(2, &mx6q_seco_q7_spi_3_data_revC);
	}

	spi_device_init();

	imx6q_add_mxc_hdmi(&hdmi_data);

	imx6q_add_anatop_thermal_imx(1, &mx6q_seco_q7_anatop_thermal_data);
	mx6q_seco_q7_fec_phy_reset(NULL);
	imx6_init_fec(fec_data);
	imx6q_add_pm_imx(0, &mx6q_seco_q7_pm_data);

#if defined(CONFIG_uSBC_EMMC_INTERFACE) // eMMC
	imx6q_add_sdhci_usdhc_imx(2, &mx6q_seco_q7_sd3_data);
#endif


	imx_add_viv_gpu(&imx6_gpu_data, &imx6q_gpu_pdata);
	imx6q_seco_q7_init_usb();

#ifdef CONFIG_Q7_SATA_AHCI
	if (cpu_is_mx6q())
		imx6q_add_ahci(0, &mx6q_seco_q7_sata_data);
#endif
	
	if (PCB_IS_REV_B || PCB_IS_REV_C){

        	imx6q_add_flexcan0(&imx6_seco_uSBC_flexcan0_pdata);

#ifdef CONFIG_Q7_CAN_TERM
		can_terminal = 0;
#else
		can_terminal = 1;
#endif	
		ret = gpio_request(MX6_SECO_uSBC_CAN_TERM, "can-termination");
                if (ret) {
                        printk ("failed to get MX6_SECO_uSBC_CAN_TERM: %d\n", ret);
                } 
                 else {
                        gpio_direction_output(MX6_SECO_uSBC_CAN_TERM, can_terminal);
#ifdef CONFIG_Q7_CAN_TERM
			printk ("flexcan: inserted CAN termination\n");
#endif 
                }	


#ifdef CONFIG_Q7_CAN_SILENT_MODE
		can_silent = 1;
#else
		can_silent = 0;
#endif
		ret = gpio_request(MX6_SECO_uSBC_CAN_S, "can-mode");
                if (ret) {
                        printk("failed to get MX6_SECO_uSBC_CAN_S: %d\n", ret);
                }
                 else {
                        gpio_direction_output(MX6_SECO_uSBC_CAN_S, can_silent);
#ifdef CONFIG_Q7_CAN_SILENT_MODE
			printk("flexcan: CAN mode: silent mode\n");
#else
			printk("flexcan: CAN mode: normal mode\n");
#endif
                }
	}
	
	imx6q_add_vpu();
	imx6q_init_audio();
	platform_device_register(&seco_q7_vmmc_reg_devices);
	imx_asrc_data.asrc_core_clk = clk_get(NULL, "asrc_clk");
	imx_asrc_data.asrc_audio_clk = clk_get(NULL, "asrc_serial_clk");
	imx6q_add_asrc(&imx_asrc_data);

	/* release USB Hub reset */
	gpio_set_value(MX6_SECO_uSBC_USB_HUB_RESET, 1);

	imx6q_add_mxc_pwm(0);
	imx6q_add_mxc_pwm(1);

	imx6q_add_mxc_pwm_backlight(0, &mx6_seco_q7_pwm_backlight_data);

	imx6q_add_otp();
	imx6q_add_viim();
	imx6q_add_imx2_wdt(0, NULL);
	imx6q_add_dma();

	imx6q_add_dvfs_core(&seco_q7_dvfscore_data);

	imx6q_add_hdmi_soc();
	imx6q_add_hdmi_soc_dai();

#if defined(CONFIG_Q7_PCIE) 
	imx6q_add_pcie(&mx6_seco_q7_pcie_data);
#endif

	clko2 = clk_get(NULL, "clko2_clk");
	if (IS_ERR(clko2))
		pr_err("can't get CLKO2 clock.\n");

	new_parent = clk_get(NULL, "osc_clk");
	if (!IS_ERR(new_parent)) {
		clk_set_parent(clko2, new_parent);
		clk_put(new_parent);
	}
	rate = clk_round_rate(clko2, 24000000);
	clk_set_rate(clko2, rate);
	clk_enable(clko2);
	imx6q_add_busfreq();

	imx6_add_armpmu();
	imx6q_add_perfmon(0);
	imx6q_add_perfmon(1);
	imx6q_add_perfmon(2);
}

extern void __iomem *twd_base;
static void __init mx6_seco_q7_timer_init(void) {
	struct clk *uart_clk;
#ifdef CONFIG_LOCAL_TIMERS
	twd_base = ioremap(LOCAL_TWD_ADDR, SZ_256);
	BUG_ON(!twd_base);
#endif
	mx6_clocks_init(32768, 24000000, 0, 0);
#ifdef CONFIG_USBC_DEBUG_UART5
	uart_clk = clk_get_sys("imx-uart.4", NULL);
	early_console_setup(UART5_BASE_ADDR, uart_clk);
#else
	uart_clk = clk_get_sys("imx-uart.0", NULL);
	early_console_setup(UART2_BASE_ADDR, uart_clk);
#endif
}

static struct sys_timer mx6_seco_q7_timer = {
	.init   = mx6_seco_q7_timer_init,
};

static void __init mx6q_seco_q7_reserve(void)
{
	phys_addr_t phys;
#if defined(CONFIG_MXC_GPU_VIV) || defined(CONFIG_MXC_GPU_VIV_MODULE)
	if (imx6q_gpu_pdata.reserved_mem_size) {
		phys = memblock_alloc_base(imx6q_gpu_pdata.reserved_mem_size,
					   SZ_4K, SZ_1G);
		memblock_free(phys, imx6q_gpu_pdata.reserved_mem_size);
		memblock_remove(phys, imx6q_gpu_pdata.reserved_mem_size);
		imx6q_gpu_pdata.reserved_mem_base = phys;
	}
#endif
}

/*
 * initialize __mach_desc_MX6_seco_q7 data structure.
 */
MACHINE_START(MX6_SECO_uSBC, "SECO i.MX6 Quad/Dual/Dual Lite/Solo uSBC Board")
	/* Maintainer: Freescale Semiconductor, Inc. */
	.boot_params = MX6_PHYS_OFFSET + 0x100,
	.fixup = fixup_mxc_board,
	.map_io = mx6_map_io,
	.init_irq = mx6_init_irq,
	.init_machine = mx6_seco_q7_board_init,
	.timer = &mx6_seco_q7_timer,
	.reserve = mx6q_seco_q7_reserve,
MACHINE_END

