# usb_relay
A usb relay

# Wire

The relay is wire to pin 12.

## Protocol

* Relay on : Send 1 byte = 41h ("A")
* Relay off: Send 1 byte = 5Ah ("Z")
* Configure timeout : Send 2 bytes ("O" and the number of seconds )
