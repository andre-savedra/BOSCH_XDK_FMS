/**
 * Generated by Eclipse Mita 0.1.0.
 * @date 2022-04-28 15:45:47
 */


#include <BCDS_Basics.h>
#include <BCDS_Bma280Utils.h>
#include <bma2x2.h>
#include <XdkSensorHandle.h>
#include <BCDS_CmdProcessor.h>
#include <BCDS_Retcode.h>
#include "MitaEvents.h"
#include "MitaExceptions.h"



/* BMA280 ISR Callback */
static void BMA280_IsrCallback(uint32_t channel, uint32_t edge);

static void BMA280_IsrCallback(uint32_t channel, uint32_t edge)
{
    BCDS_UNUSED(channel);
    BCDS_UNUSED(edge);
}



Retcode_T SensorAccelerometer_Setup(void)
{
	Retcode_T retcode = RETCODE_OK;
	
	/* Extract BMA280 Handle from Advanced API */
	Bma280Utils_InfoPtr_T bma280UtilityCfg = xdkAccelerometers_BMA280_Handle->SensorPtr;
	
	/* Register Callback */
	bma280UtilityCfg->ISRCallback = BMA280_IsrCallback;
	
	retcode = Bma280Utils_initialize(bma280UtilityCfg);
	if(retcode != RETCODE_OK)
	{
	    return retcode;
	}
	
	/* Do soft reset and wait 2-3 ms */
	// bma2x2_soft_rst();
	/* Overwrite settings */
	
	/* Block One: Basic Settings */
	/* Required: Set power mode to normal for configuration */
	bma2x2_set_power_mode(BMA2x2_MODE_NORMAL);
	
	/* Set Bandwidth */
	bma2x2_set_bw(BMA2x2_BW_500HZ);
	
	/* Setting to filtered low-bandwidth */
	bma2x2_set_high_bw(1);
	
	/* Set Range */
	bma2x2_set_range(BMA2x2_RANGE_8G);
	
	/* Set Sleep Duration */
	//bma2x2_set_sleep_durn(BMA2x2_SLEEP_DURN_1S);
	//bma2x2_set_sleep_timer_mode(0);
	/* Set Interrupt latch and level */
	
	/* Output stages */
	bma2x2_set_latch_intr(BMA2x2_LATCH_DURN_250MS);
	bma2x2_set_intr_level(BMA2x2_INTR1_LEVEL, ACTIVE_LOW);
	bma2x2_set_intr_level(BMA2x2_INTR2_LEVEL, ACTIVE_LOW);
	bma2x2_set_intr_output_type(BMA2x2_INTR1_OUTPUT, PUSS_PULL);
	bma2x2_set_intr_output_type(BMA2x2_INTR2_OUTPUT, PUSS_PULL);
	
	/* Set power mode to target value */
	bma2x2_set_power_mode(BMA2x2_MODE_NORMAL);
	
	/* Group 1: INTR1 Settings */
	bma2x2_set_intr_low_g(BMA2x2_INTR1_LOW_G, INTR_ENABLE);
	bma2x2_set_intr_high_g(BMA2x2_INTR1_HIGH_G, INTR_ENABLE);
	bma2x2_set_intr_slope(BMA2x2_INTR1_SLOPE, INTR_ENABLE);
	bma2x2_set_intr_slow_no_motion(BMA2x2_INTR1_SLOW_NO_MOTION, INTR_ENABLE);
	bma2x2_set_intr_double_tap(BMA2x2_INTR1_DOUBLE_TAP, INTR_ENABLE);
	bma2x2_set_intr_single_tap(BMA2x2_INTR1_SINGLE_TAP, INTR_ENABLE);
	bma2x2_set_intr_orient(BMA2x2_INTR1_ORIENT, INTR_ENABLE);
	bma2x2_set_intr_flat(BMA2x2_INTR1_FLAT, INTR_ENABLE);
	bma2x2_set_new_data(BMA2x2_INTR1_NEWDATA, INTR_ENABLE);
	bma2x2_set_intr1_fifo_wm(INTR_ENABLE);
	bma2x2_set_intr1_fifo_full(INTR_ENABLE);
	
	/* Group 2: INTR2 Settings */
	bma2x2_set_intr_low_g(BMA2x2_INTR2_LOW_G, INTR_DISABLE);
	bma2x2_set_intr_high_g(BMA2x2_INTR2_HIGH_G, INTR_DISABLE);
	bma2x2_set_intr_slope(BMA2x2_INTR2_SLOPE, INTR_DISABLE);
	bma2x2_set_intr_slow_no_motion(BMA2x2_INTR2_SLOW_NO_MOTION, INTR_DISABLE);
	bma2x2_set_intr_double_tap(BMA2x2_INTR2_DOUBLE_TAP, INTR_DISABLE);
	bma2x2_set_intr_single_tap(BMA2x2_INTR2_SINGLE_TAP, INTR_DISABLE);
	bma2x2_set_intr_orient(BMA2x2_INTR2_ORIENT, INTR_DISABLE);
	bma2x2_set_intr_flat(BMA2x2_INTR2_FLAT, INTR_DISABLE);
	bma2x2_set_new_data(BMA2x2_INTR2_NEWDATA, INTR_DISABLE);
	bma2x2_set_intr2_fifo_wm(INTR_DISABLE);
	bma2x2_set_intr2_fifo_full(INTR_DISABLE);
	
	
	
	
	
	
	/* orientation */
	
	/* flat */
	
	/* New data */
	bma2x2_set_source(BMA2x2_SOURCE_DATA, 0);
	
	return NO_EXCEPTION;
}

Retcode_T SensorAccelerometer_Enable(void)
{
	/* Enable Active Sensor Events */
	
	
	return NO_EXCEPTION;
}

Retcode_T SensorAccelerometer_ReadXYZ(struct bma2x2_accel_data* result)
{    
    // read sensor data
    BMA2x2_RETURN_FUNCTION_TYPE bmaReadRc = bma2x2_read_accel_xyz(result);
    if(bmaReadRc != 0)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    // remap to XDK axis alignment
    AxisRemap_Data_T axisRemapping = { INT32_C(0), INT32_C(0), INT32_C(0) };
    axisRemapping.x = result->x;
    axisRemapping.y = result->y;
    axisRemapping.z = result->z;
    Retcode_T remappingStatus = Bma280Utils_remapAxis((Bma280Utils_InfoPtr_T) xdkAccelerometers_BMA280_Handle->SensorPtr, &axisRemapping);
    
    // store in result
    if (RETCODE_OK == remappingStatus)
    {
        result->x = axisRemapping.x;
        result->y = axisRemapping.y;
        result->z = axisRemapping.z;
        return RETCODE_OK;
    } else {
        return remappingStatus;
    }
}
