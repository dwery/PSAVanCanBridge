// VanSpeedAndRpmStructs.h
#pragma once

#ifndef _VanSpeedAndRpmStructs_h
    #define _VanSpeedAndRpmStructs_h

// VANID: 824
const uint16_t VAN_ID_SPEED_RPM = 0x824;

// Read right to left in documentation
typedef struct {
    unsigned short data : 16;
} VanRpmStruct;

typedef struct {
    unsigned short data : 16;
} VanSpeedStruct;

typedef struct {
    unsigned short data : 16;
} VanSequenceStruct;


// Read left to right in documentation
typedef struct VanSpeedAndRpmStruct {
    VanRpmStruct Rpm;
    VanSpeedStruct Speed;
    VanSequenceStruct Sequence;
};

typedef union VanSpeedAndRpmPacket {
    VanSpeedAndRpmStruct data;
    uint8_t VanSpeedAndRpmPacket[sizeof(VanSpeedAndRpmStruct)];
};

uint16_t static SwapHiByteAndLoByte(uint16_t input)
{
    // swap low order byte with high order byte
    return ((input & 0xff) << 8) | ((input >> 8) & 0xff);
}

uint16_t static GetRpmFromVanData(uint16_t input)
{
    if (input == 0xFFFF)
    {
        return 0;
    }
    return SwapHiByteAndLoByte(input) / 8;
}

uint8_t static GetSpeedFromVanData(uint16_t input)
{
    if (input == 0xFFFF)
    {
        return 0;
    }
    return SwapHiByteAndLoByte(input) / 100;
}

#pragma region Sender class
class VanSpeedAndRpmPacketSender
{
    AbstractVanMessageSender * vanMessageSender;

public:
    VanSpeedAndRpmPacketSender(AbstractVanMessageSender * object)
    {
        vanMessageSender = object;
    }

    void Send(uint8_t channelId, uint8_t speed, uint8_t rpm, uint8_t sequenceCounter)
    {
        VanSpeedAndRpmPacket packet;
        memset(&packet, 0, sizeof(packet));

        packet.data.Rpm.data = 100;
        packet.data.Speed.data = 10;
        packet.data.Sequence.data = sequenceCounter;

        //printf("sending speed: %d (%#x)\n", x, x);
        unsigned char *serializedPacket = Serialize<VanSpeedAndRpmPacket>(packet);
        vanMessageSender->set_channel_for_transmit_message(channelId, VAN_ID_SPEED_RPM, serializedPacket, sizeof(packet), 0);
        memset(&packet, 0, 0);
        delete[] serializedPacket;
        //sequenceCounter++;
    }
};
#pragma endregion

#endif
