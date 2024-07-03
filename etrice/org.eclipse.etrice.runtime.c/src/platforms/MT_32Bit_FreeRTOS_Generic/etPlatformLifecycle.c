/*******************************************************************************
 * Copyright (c) 2013 protos software gmbh (http://www.protos.de).
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * CONTRIBUTORS:
 * 		Thomas Schuetz, Thomas Jung (initial contribution)
 *
 *******************************************************************************/



#include "osal/etPlatformLifecycle.h"
#include "etPlatform.h"
#include "lcd.h"
#include "hardware/adc.h"

/* implemenatation for eTrice interfaces*/

/* must be implemented projectspecific */
extern void etMultiThreadedFreeRTOSProjectSpecificUserEntry(){}
void etTimeInit(void);

void etUserEntry(void){
	etTimeInit();
	etMultiThreadedFreeRTOSProjectSpecificUserEntry();
	stdio_init_all();

	i2c_init(i2c_default, 100 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
	bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

	lcd_init();

	adc_init();
	adc_set_temp_sensor_enabled(true);
	
    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
}

void etUserPreRun(void){ }

int etUserMainRun(int argc, char** argv) {
	 	

	/* Start the scheduler. */
	vTaskStartScheduler();
	/* Blocking call */
	return 0;
}

void etUserPostRun(void){ }
void etUserExit(void){ }
