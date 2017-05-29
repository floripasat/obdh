from input_RawData import text_treatment
from functions import *
import xlwt

workbook = xlwt.Workbook()
worksheet = workbook.add_sheet(u'uG_Mission')

worksheet.write(0,0, u"obdh_clock")
worksheet.write(1,0, u"sysclock_s")
worksheet.write(1,1, u"sysclock_ms")

worksheet.write(0,2, u"obdh_temperature")

worksheet.write(0,3, u"obdh_statuscode")

worksheet.write(0,4, u"accelerometer")
worksheet.write(1,4, u"imu_acc_x")
worksheet.write(1,5, u"imu_acc_y")
worksheet.write(1,6, u"imu_acc_z")

worksheet.write(0,7, u"gyroscope")
worksheet.write(1,7, u"imu_gyr_x")
worksheet.write(1,8, u"imu_gyr_y")
worksheet.write(1,9, u"imu_gyr_z")

worksheet.write(0,10, u"radio_counters")
worksheet.write(1,10, u"counter1")
worksheet.write(1,11, u"counter2")

worksheet.write(0,12, u"eps_current")

worksheet.write(0,13, u"obdh_voltage_bat")
worksheet.write(1,13, u"voltage_bat1")
worksheet.write(1,14, u"voltage_bat2")

worksheet.write(0,15, u"eps_temperature")

worksheet.write(0,16, u"eps_current_acc")

worksheet.write(0,17, u"eps_batmon_reg")

worksheet.write(0,18, u"crc_field")

i = 1
y = 0
w = 0

file_name = "uG-FloripaSat-RawData.txt"

file = open(file_name, 'r')
text = file.readlines()

for j, l in enumerate(text):
	file_len = j

sub_division = int(file_len / 10)

while( y <= file_len):
	data = text_treatment(start = y, end = y+sub_division) 
	
	byte_H = 0
	byte_L = 0

	for w in range(0, len(data)):	
		x = 0

		# obdh clocks
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		obdh_sysclock_s = byte_concatenation_HL(byte_H, byte_L)
		worksheet.write(i+1,0, obdh_sysclock_s)
		x = x + 2

		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		obdh_sysclock_ms = byte_concatenation_HL(byte_H, byte_L)
		worksheet.write(i+1,1, obdh_sysclock_ms)
		x = x + 2

		# odbh internal temperature
		# precisa achar qual é a conversao

		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		obdh_internal_temperature = obdh_temperature_raw_to_degrees(byte_H, byte_L)
		worksheet.write(i+1,2, obdh_internal_temperature)
		x = x + 2

		# statuscode

		obdh_statuscode = (data[w])[x] 
		worksheet.write(i+1,3, obdh_statuscode)
		x = x + 1	

		#accelerometer

		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_acc_x = imu_acc_raw_to_g(byte_H, byte_L)
		worksheet.write(i+1,4, imu_acc_x)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_acc_y = imu_acc_raw_to_g(byte_H, byte_L)
		worksheet.write(i+1,5, imu_acc_y)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_acc_z = imu_acc_raw_to_g(byte_H, byte_L)
		worksheet.write(i+1,6, imu_acc_z)
		x = x + 2

		#gyroscope
	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_gyr_x = imu_gyr_raw_to_ds(byte_H, byte_L)
		worksheet.write(i+1,7, imu_gyr_x)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_gyr_y = imu_gyr_raw_to_ds(byte_H, byte_L)
		worksheet.write(i+1,8, imu_gyr_y)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		imu_gyr_z = imu_gyr_raw_to_ds(byte_H, byte_L)
		worksheet.write(i+1,9, imu_gyr_z)
		x = x + 2

		# radio counters
	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		radio_counter1 = byte_concatenation_HL(byte_H, byte_L)
		worksheet.write(i+1,10, radio_counter1)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		radio_counter2 = byte_concatenation_HL(byte_H, byte_L)
		worksheet.write(i+1,11, radio_counter2)
		x = x + 2

		#EPS current
	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		eps_current = eps_current_raw_to_mA(byte_H, byte_L)
		worksheet.write(i+1,12, eps_current)
		x = x + 2

		#baterry
	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		eps_voltage_bat1 = eps_voltage_bat_raw_to_v(byte_H, byte_L)
		worksheet.write(i+1,13, eps_voltage_bat1)
		x = x + 2

	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		eps_voltage_bat2 = eps_voltage_bat_raw_to_v(byte_H, byte_L)
		worksheet.write(i+1,14, eps_voltage_bat2)
		x = x + 2

		#EPS temperature
	
		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		eps_temperature = eps_temperature_raw_to_degrees(byte_H, byte_L)
		worksheet.write(i+1,15, eps_temperature)
		x = x + 2

		#EPS current_acc

		byte_H = (data[w])[x] 
		byte_L = (data[w])[x+1]
		eps_current_acc = eps_current_acc_raw_to_mA(byte_H, byte_L)
		worksheet.write(i+1,16, eps_current_acc)
		x = x + 2

		#EPS BATMON REG
	
		eps_batmon_reg = (data[w])[x] 
		worksheet.write(i+1,17, eps_batmon_reg)
		x = x + 1

		#CRC FIELD	
	
		crc_field = (data[w])[x]
		worksheet.write(i+1,18, crc_field)
		x = x + 1	

		i = i + 1

	y = y + sub_division
	data = []


workbook.save('uG_converted.xls')