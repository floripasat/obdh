IMU_ACC_RANGE = 16.0
IMU_GYR_RANGE =  250.0

CALADC12_15V_30C = 2145
CALADC12_15V_85C = 2508


def byte_concatenation_HL(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	byte_total = (byte_H << 8) + byte_L
	return byte_total

def obdh_temperature_raw_to_degrees(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	obdh_temperature_raw = (byte_H << 8) + byte_L
	obdh_temperature = ((obdh_temperature_raw - CALADC12_15V_30C) * (85.0 - 30.0)) /(CALADC12_15V_85C - CALADC12_15V_30C) + 30.0
	return obdh_temperature

def imu_acc_raw_to_g(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)

	byte_2 = (byte_H << 8 | byte_L)

	if (byte_2 >= 0x1000):
		byte_2 = 0xFFFF - byte_2

	byte_total = ((byte_2 * IMU_ACC_RANGE) / 32768.0)
	return byte_total

def imu_gyr_raw_to_ds(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)

	byte_2 = (byte_H << 8 | byte_L)

	if (byte_2 >= 0x1000):
		byte_2 = 0xFFFF - byte_2
		
	byte_total = ((byte_2 * IMU_GYR_RANGE) / 32768.0)
	return byte_total

def eps_current_raw_to_mA(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	byte_total = ((byte_H << 8) + byte_L) * (0.0000015625 / 0.015)
	return byte_total

def eps_voltage_bat_raw_to_v(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	byte_total = ((byte_H << 8) + byte_L) * 0.004886
	return byte_total

def eps_temperature_raw_to_degrees(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	byte_total = ((byte_H << 8) + byte_L) * 0.125
	return byte_total

def eps_current_acc_raw_to_mA(byte_H, byte_L):
	byte_H = int(byte_H, base=16)
	byte_L = int(byte_L, base=16)
	byte_total = ((byte_H << 8) + byte_L) * (0.00000625 / 0.015)
	return byte_total