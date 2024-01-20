//
// Created by sheik on 1/17/2024.
//

#ifndef EVENT_H
#define EVENT_H

#pragma once
enum event {
    ev_any,
    ev_stop,
    ev_reset,
    ev_ready,
    ev_be_leader,
    ev_be_follower,
    ev_leader_found,
    ev_no_leader_found
};

#endif
