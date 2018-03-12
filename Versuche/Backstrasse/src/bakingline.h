/*
 * bakingline.h
 *
 *  Created on: Dec 21, 2017
 *      Author: lab_mps_stud07
 */

#ifndef BAKINGLINE_H_
#define BAKINGLINE_H_

/***************Define Section**********************************/
#define MFB1 0
#define MD1W 1
#define MD1F 2
#define MFBD1 3
#define MFBW 4
#define MD2M 5
#define MD2W 6
#define MFBD2 7
#define MFBM 8
#define MFBO 9
#define MFS1 10
#define MFS2 11
#define MFS3 12
#define MMG 13
#define MMAP 14

#define MMAN 0
#define OFEN 1

#define SLE 0
#define SIF1 1
#define SID1 2
#define STD1B 3
#define STD1W 4
#define SIFW 5
#define SID2 6
#define STD2W 7
#define STD2M 8
#define SIFM 9
#define SLS1 10
#define SLS2 11
#define SLS3 12
#define STMA 13
#define PT100_1 14
#define PT100_2 15

#define SIO 0
#define STMG 1
#define SLA 2

#define ALPHA 0.5
#define BAKING_TEMP 35

/*******************End of Define Section*******************************/

//**************************State Definition***********************
enum box_count_states {
	SLE_LOW, SLE_HIGH
};
enum first_band_states {
	IDLE_FB1, MOVE_IN_FB1, WAIT_OUT_FB1, MOVE_OUT_FB1, MORE_BOX_OUT_FB1
};
enum first_turntable_states {
	START_FT1,
	IDLE_FT1,
	MOVE_IN_FT1,
	MOVE_CCW_FT1,
	MOVE_CW_FT1,
	WAIT_OUT_FT1,
	MOVE_OUT_FT1
};
enum second_turntable_states {
	START_FT2,
	IDLE_FT2,
	MOVE_IN_FT2,
	MOVE_CCW_FT2,
	MOVE_CW_FT2,
	WAIT_OUT_FT2,
	MOVE_OUT_FT2
};
enum silo_states {
	IDLE_SILO, MOVE_IN_SILO, FILL_SILO, WAIT_OUT_SILO, MOVE_OUT_SILO
};
enum mixer_states {
	START_MIXER,
	IDLE_MIXER,
	MOVE_IN_MIXER,
	MOVE_DOWN_MIXER,
	MIX_MIXER,
	MOVE_UP_MIXER,
	PREHEAT_OVEN,
	WAIT_OUT_MIXER,
	MOVE_OUT_MIXER
};
enum oven_states {
	IDLE_OVEN,
	WAIT_IN_OVEN,
	MOVE_IN_OVEN,
	BAKE_OVEN,
	MOVE_OUT_OVEN
};
//***************************End of State Definition******************

/****Global variables***********************/

int box_count;
int silo_time;
int mixer_time;
int oven_time;
int pwm_value_oven;

float voltage;
float resistance;
float temperature;
float temperature_old;

enum box_count_states box_count_current_state;
enum first_band_states first_band_current_state;
enum first_turntable_states first_turntable_current_state;
enum second_turntable_states second_turntable_current_state;
enum silo_states silo_current_state;
enum mixer_states mixer_current_state;
enum oven_states oven_current_state;
/******************************************/

/************FUNCTIONS**********************/
void set(int motor);
void reset(int motor);
int get(int sensor);

void set24(int motor);
void reset24(int motor);
int get24(int sensor);

void box_count_fsm();
void first_band_fsm();
void fist_turntable_fsm();
void silo_fsm();
void second_turntable_fsm();
void mixer_fsm();
void oven_fsm();
/*******************************************/

#endif /* BAKINGLINE_H_ */
