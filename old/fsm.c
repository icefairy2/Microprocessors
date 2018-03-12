#include "bakingline.h"

/* Finite state machine for the light sensor at the start of the system.
 * It counts how many boxes go in
 */
void box_count_fsm() {
    switch (box_count_current_state) {
    case SLE_LOW:
        if (get(SLE)) { //light sensor at the beginning on
            box_count++;
            box_count_current_state = SLE_HIGH;
        }
        break;
    case SLE_HIGH:
        if (!get(SLE)) { //light sensor at the beginning off
            box_count_current_state = SLE_LOW;
        }
        break;
    }
}

/*
 * Finite state machine for the first band of the system
 */
void first_band_fsm() {
    switch (first_band_current_state) {
    case IDLE_FB1:
        if (box_count > 0) {
            set(MFB1); //start first rolling band
            first_band_current_state = MOVE_IN_FB1;
        }
        break;
    case MOVE_IN_FB1:
        if (get(SIF1)) { //sensor at end of first rolling band on
            reset(MFB1); //stop first rolling band
            first_band_current_state = WAIT_OUT_FB1;
        }
        break;
    case WAIT_OUT_FB1:
        if (first_turntable_current_state == IDLE_FT1) {
            set(MFB1); //start first rolling band
            first_band_current_state = MORE_BOX_OUT_FB1;

        }
        break;
    case MORE_BOX_OUT_FB1:
        if (!get(SIF1)) { //sensor on first rolling band off
            box_count--;
            first_band_current_state = MOVE_OUT_FB1;
        }
        break;
    case MOVE_OUT_FB1:
        if (get(SIF1)) { //sensor on first rolling band on
            reset(MFB1); //stop first rolling band
            first_band_current_state = WAIT_OUT_FB1;
        } else if (get(SID1)) { //sensor on first turn table on (there is a box on the turn table)
            if (box_count == 0) {
                reset(MFB1); //stop first rolling band
                first_band_current_state = IDLE_FB1;
            } else {
                first_band_current_state = MOVE_IN_FB1;
            }

        }
        break;
    }
}

/*
 * Finite state machine for the first turn table of the system
 */
void fist_turntable_fsm() {
    switch (first_turntable_current_state) {
    case START_FT1:
        if (!get(STD1B)) { //sensor of first turn table facing the first rolling band off
            set(MD1F); //turn counter clockwise
        } else {
            reset(MD1F); //stop turning counter clockwise
            first_turntable_current_state = IDLE_FT1;
        }

    case IDLE_FT1:
        if (first_band_current_state == MOVE_OUT_FB1) {
            set(MFBD1); //start rolling band on first turn table
            first_turntable_current_state = MOVE_IN_FT1;
        }
        break;
    case MOVE_IN_FT1:
        if (get(SID1)) { //sensor on first turn table on
            reset(MFBD1); //stop rolling band on first turn table
            set(MD1W); //turn clockwise
            first_turntable_current_state = MOVE_CW_FT1;
        }
        break;
    case MOVE_CW_FT1:
        if (get(STD1W)) { //sensor of first turn table facing the silo rolling band on
            reset(MD1W); //stop turning clockwise
            first_turntable_current_state = WAIT_OUT_FT1;
        }
        break;
    case WAIT_OUT_FT1:
        if (silo_current_state == IDLE_SILO) {
            set(MFBD1); //start rolling band on first turn table
            first_turntable_current_state = MOVE_OUT_FT1;
        }
        break;
    case MOVE_OUT_FT1:
        if (get(SIFW)) { //sensor on silo middle on (box got on the silo)
            reset(MFBD1); //stop rolling band on first turn table
            set(MD1F); //turn counter clockwise
            first_turntable_current_state = MOVE_CCW_FT1;
        }
        break;
    case MOVE_CCW_FT1:
        if (get(STD1B)) { //sensor of first turn table facing the first rolling band on
            reset(MD1F); //stop turning counter clockwise
            first_turntable_current_state = IDLE_FT1;
        }
        break;
    }
}

/*
 * Finite state machine for the silo of the system
 */
void silo_fsm() {
    switch (silo_current_state) {
    case IDLE_SILO:
        if (first_turntable_current_state == MOVE_OUT_FT1) {
            set(MFBW); //start silo rolling band
            silo_current_state = MOVE_IN_SILO;
        }
        break;
    case MOVE_IN_SILO:
        if (get(SIFW)) { //sensor on middle of silo on
            reset(MFBW); //stop silo rolling band
            silo_time = 0; //reset timer
            set(MFS1); //start pouring silo 1
            set(MFS2); //start pouring silo 2
            set(MFS3); //start pouring silo 3
            silo_current_state = FILL_SILO;
        }
        break;
    case FILL_SILO:
        silo_time++; //increase timer value for every loop
        if (silo_time == 1000) {
            reset(MFS1); //at 2 sec stop pouring first silo
        } else if (silo_time == 1500) {
            reset(MFS2); //at 3 sec stop pouring second silo
        } else if (silo_time == 2500) {
            reset(MFS3); //at 5 sec stop pouring third silo
            silo_current_state = WAIT_OUT_SILO;
        }
        break;
    case WAIT_OUT_SILO:
        if (second_turntable_current_state == IDLE_FT2) {
            set(MFBW); //start silo rolling band
            silo_current_state = MOVE_OUT_SILO;
        }
        break;
    case MOVE_OUT_SILO:
        if (get(SID2)) { //sensor on second turn table on
            reset(MFBW); //stop silo rolling band
            silo_current_state = IDLE_SILO;
        }
        break;
    }
}

/*
 * Finite state machine for the second turn table of the system
 */
void second_turntable_fsm() {
    switch (second_turntable_current_state) {
    case START_FT2:
        if (!get(STD2W)) { //sensor of second turn table facing the silo rolling band off
            set(MD2W); //turn counter clockwise
        } else {
            reset(MD2W); //stop turning counter clockwise
            second_turntable_current_state = IDLE_FT2;
        }
        break;
    case IDLE_FT2:
        if (silo_current_state == MOVE_OUT_SILO) {
            set(MFBD2); //start rolling band on second turn table
            second_turntable_current_state = MOVE_IN_FT2;
        }
        break;
    case MOVE_IN_FT2:
        if (get(SID2)) { //sensor on second turn table on
            reset(MFBD2); //stop rolling band on second turn table
            set(MD2M); //turn clockwise
            second_turntable_current_state = MOVE_CW_FT2;
        }
        break;
    case MOVE_CW_FT2:
        if (get(STD2M)) { //sensor of second turn table facing the mixer rolling band on
            reset(MD2M); //stop turning clockwise
            second_turntable_current_state = WAIT_OUT_FT2;
        }
        break;
    case WAIT_OUT_FT2:
        if (!get(SIFM)) {
            set(MFBD2); //start rolling band on second turn table
            second_turntable_current_state = MOVE_OUT_FT2;
        }
        break;
    case MOVE_OUT_FT2:
        if (get(SIFM)) {
            reset(MFBD2); //stop rolling band on second turn table
            set(MD2W); //turn counter clockwise
            second_turntable_current_state = MOVE_CCW_FT2;
        }
        break;
    case MOVE_CCW_FT2:
        if (get(STD2W)) { //sensor of second turn table facing the silo rolling band on
            reset(MD2W); //stop turning counter clockwise
            second_turntable_current_state = IDLE_FT2;
        }
        break;
    }
}

/*
 * Finite state machine for the mixer of the system
 */
void mixer_fsm() {
    switch (mixer_current_state) {
    case START_MIXER:
        if (!get24(STMG)) {//checking if the mixer is in the right position
            set(MMG); //if it is not, we are moving it to the right position
        } else {
            reset(MMG); //when it gets to the right position we stop the motor
            mixer_current_state = IDLE_MIXER;
        }
        break;
    case IDLE_MIXER:
        if (second_turntable_current_state == MOVE_OUT_FT2) { //if turntable 2 is giving us the box
            set(MFBM); //start moving the band under the mixer
            mixer_current_state = MOVE_IN_MIXER;
        }
        break;
    case MOVE_IN_MIXER:
        if (get(SIFM)) { //if the box gets in the position
            reset(MFBM); //stop the band under the mixer
            set(MMAP); //start the mixer moving down
            mixer_current_state = MOVE_DOWN_MIXER;
        }
        break;
    case MOVE_DOWN_MIXER:
        if (get(STMA)) { //if the mixer gets the working position
            reset(MMAP); //stop moving the mixer down
            set24(MMAN); //start the mixing
            mixer_time = 0;
            mixer_current_state = MIX_MIXER;
        }
        break;
    case MIX_MIXER:
        mixer_time++;
        if (mixer_time == MIXING_TIME) { //when the 5 sec finishes
            reset24(MMAN); //stop the mixing
            set(MMG); //start the mixer moving up the beginning position
            mixer_current_state = MOVE_UP_MIXER;
        }
        break;
    case MOVE_UP_MIXER:
        if (get24(STMG)) { //when the mixer gets the beginning position
            reset(MMG); //stop moving the mixer up
            mixer_current_state = PREHEAT_OVEN;
        }
        break;
    case PREHEAT_OVEN:
        if (oven_current_state != BAKE_OVEN) {
            if (temperature >= BAKING_TEMP) {
                pwm_value_oven = 0;
                mixer_current_state = WAIT_OUT_MIXER;
            }
            else {
                pwm_value_oven = 10000;
            }
        }
        break;
    case WAIT_OUT_MIXER:
        if (get24(SIO)) { // if the oven avaliable
            set(MFBM); // start moving the band under the mixer
            mixer_current_state = MOVE_OUT_MIXER;
        }
        break;
    case MOVE_OUT_MIXER:
        if (!get24(SIO)) { //when the box gets to the position in the oven
            reset(MFBM); // stop the band under the mixer
            mixer_current_state = IDLE_MIXER;
        }
        break;
    }
}

/*
 * Finite state machine for the oven of the system
 */
void oven_fsm() {
    switch (oven_current_state) {
    case IDLE_OVEN:
        if (!get24(SLA)){  //if finish area clear (we don't want to burn the cake)
            oven_current_state = WAIT_IN_OVEN;
        }
    case WAIT_IN_OVEN:
        if (mixer_current_state == MOVE_OUT_MIXER) {
            //check if the oven is ready
            set(MFBO);
            oven_current_state = MOVE_IN_OVEN;
        }
        break;
    case MOVE_IN_OVEN:
        if (!get24(SIO)) {
            reset(MFBO);
            oven_time = 0;
            //set24(OFEN);
            oven_current_state = BAKE_OVEN;
        }
        break;
    case BAKE_OVEN:

        oven_time++;


        if((BAKING_TEMP - temperature) < 5){
            pwm_value_oven = 2000 * (int)(BAKING_TEMP - temperature);
            if(pwm_value_oven >= 10000){
                pwm_value_oven = 10000;
            }
            else if(temperature >= BAKING_TEMP){
                pwm_value_oven = 0;
            }
        }
        else{
            pwm_value_oven = 10000;
        }
        //the P controller
        //pwm_value_oven = 100 * (int)(BAKING_TEMP - temperature);  //not sure about the 10
        /*pwm_value_oven = (temperature >= 10000)? 10000 : pwm_value_oven;
        pwm_value_oven = (temperature >= BAKING_TEMP)? 0 : pwm_value_oven;*/




        if (oven_time == BAKING_TIME) { //when the oven time finishes
            pwm_value_oven = 0;
            set(MFBO);
            oven_current_state = MOVE_OUT_OVEN;
        }
        break;
    case MOVE_OUT_OVEN:
        if (get24(SLA)) {
            reset(MFBO);
            oven_current_state = IDLE_OVEN;
        }
        break;
    }
}
