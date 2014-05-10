/*
  RCOutputFirmata.h - Firmata library

  Version: 1.0-SNAPSHOT
  Date:    2014-05-10
  Author:  git-developer ( https://github.com/git-developer )
   
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef RCOutputFirmata_h
#define RCOutputFirmata_h

#include <utility/FirmataFeature.h>
#include <RCSwitch.h>

/* Subcommands */
#define UNKNOWN                 0x00

#define CONFIG_PROTOCOL         0x11
#define CONFIG_PULSE_LENGTH     0x12
#define CONFIG_REPEAT_TRANSMIT  0x14

#define CODE_TRISTATE           0x21
#define CODE_LONG               0x22
#define CODE_CHAR               0x24

/* Tristate bit values */
#define TRISTATE_0              0x00
#define TRISTATE_F              0x01
#define TRISTATE_RESERVED       0x02
#define TRISTATE_1              0x03

class RCOutputFirmata:public FirmataFeature
{

public:
  boolean handlePinMode(byte pin, int mode);
  void handleCapability(byte pin);
  
  /**
   * When a command was executed successfully,
   * it will be mirrored back to the host.
   * This may be used to track errors.
   */
  boolean handleSysex(byte command, byte argc, byte* argv);
  void reset();

private:

  /** 1 sender per pin */
  RCSwitch* senders[TOTAL_PINS];
  
  /**
   * Initializes a sender for a pin.
   *
   * @param pin Pin to connect to a sender
   */
  void attach(byte pin);
  
  /**
   * Removes the sender for a pin.
   *
   * @param pin Pin that is connected to a sender
   */
  void detach(byte pin);

  /**
   * Sends a tristate code via RCSwitch.
   *
   * @param sender RC sender
   * @param data   Tristate bits
   * @param length Length of data in bytes
   * @return Number of processed bytes
   */
  byte sendTristate(RCSwitch *sender, byte *data, byte length);

  /**
   * Sends a long value via RCSwitch.
   *
   * @param sender RC sender
   * @param data data[0..1]: number of bits to send; data[2..5]: long value
   * @return 
   */
  byte sendLong(RCSwitch *sender, byte *data);

  /**
   * Sends a character string via RCSwitch.
   *
   * @param sender RC sender
   * @param data   character string to send
   * @return Number of sent characters
   */
  byte sendString(RCSwitch *sender, byte *data);

  /**
   * Converts a byte[] with tristate bits to a string.
   *
   * @param tristateBytes byte[] with 4 tristate bits per byte
   * @param length        Length of the byte[]
   * @param tristateCode  Target for the string
   * @return Number of written characters
   */
  byte unpack(byte *tristateBytes, byte length, char* tristateCode);
  
  /**
   * Converts a string with tristate bits to a byte[].
   *
   * @param tristateCode  String with tristate bits ('0', '1', 'F')
   * @param length        Length of the string
   * @param tristateBytes Target for the tristate bits
   *                        with 4 tristate bits per byte
   * @return Number of written bytes
   */
  byte pack(char* tristateCode, byte length, byte *tristateBytes);

  /**
   * Extracts a tristate bit from a byte.
   *
   * @param tristateByte  A byte of 4 tristate bits
   * @param index         Index of the tristate bit to read (0..3)
   * @return Char representation of the requested tristate bit
   */
  char getTristateChar(byte tristateByte, byte index);

  /**
   * Sets a tristate bit within a byte.
   *
   * @param tristateByte  A byte of 4 tristate bits
   * @param index         Index of the tristate bit to write (0..3)
   * @param char          Tristate bit to write
   * @return The given byte with the requested tristate bit set
   */
  byte setTristateBit(byte tristateByte, byte index, char tristateChar);

  /**
   * Send a message to the firmata host.
   *
   * @param subcommand Details about the message
   *                     (see the constants defined above)
   * @param pin        Pin that corresponds to the message
   * @param length     Message length
   * @param data       Message content
   */  
  void sendMessage(byte subcommand, byte pin, byte length, byte *data);

};

#endif
