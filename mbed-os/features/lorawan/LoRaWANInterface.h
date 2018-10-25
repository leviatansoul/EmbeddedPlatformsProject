/**
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LORAWANINTERFACE_H_
#define LORAWANINTERFACE_H_

#include "platform/Callback.h"
#include "platform/ScopedLock.h"
#include "LoRaWANStack.h"
#include "LoRaRadio.h"
#include "LoRaWANBase.h"

class LoRaWANInterface: public LoRaWANBase {

public:

    /** Constructs a LoRaWANInterface using the LoRaWANStack instance underneath.
     *
     * Currently, LoRaWANStack is a singleton and you should only
     * construct a single instance of LoRaWANInterface.
     *
     */
    LoRaWANInterface(LoRaRadio &radio);

    virtual ~LoRaWANInterface();

    /** Initialize the LoRa stack.
     *
     * You must call this first to be able to use the LoRa stack.
     *
     * @param ev_queue A pointer to EventQueue provided by the application.
     *
     * @return         0 on success, a negative error code on failure.
     */
    virtual lorawan_status_t initialize(events::EventQueue *ev_queue);

    /** Connect OTAA or ABP using Mbed-OS config system
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * You need to configure the connection properly via the Mbed OS configuration
     * system.
     *
     * When connecting via OTAA, the return code for success (LORAWAN_STATUS_CONNECT_IN_PROGRESS) is negative.
     * However, this is not a real error. It tells you that the connection is in progress and you will
     * be notified of the completion via an event. By default, after the Join Accept message
     * is received, base stations may provide the node with a CF-List that replaces
     * all user-configured channels except the Join/Default channels. A CF-List can
     * configure a maximum of five channels other than the default channels.
     *
     * To configure more channels, we recommend that you use the `set_channel_plan()` API after the connection.
     * By default, the PHY layers configure only the mandatory Join channels. The retransmission back-off restrictions
     * on these channels are severe and you may experience long delays or even failures in the confirmed traffic.
     * If you add more channels, the aggregated duty cycle becomes much more relaxed as compared to the Join (default) channels only.
     *
     * **NOTES ON RECONNECTION:**
     * Currently, the Mbed OS LoRaWAN implementation does not support non-volatile
     * memory storage. Therefore, the state and frame counters cannot be restored after
     * a power cycle. However, if you use the `disconnect()` API to shut down the LoRaWAN
     * protocol, the state and frame counters are saved. Connecting again would try to
     * restore the previous session. According to the LoRaWAN 1.0.2 specification, the frame counters are always reset
     * to zero for OTAA and a new Join request lets the network server know
     * that the counters need a reset. The same is said about the ABP but there
     * is no way to convey this information to the network server. For a network
     * server, an ABP device is always connected. That's why storing the frame counters
     * is important, at least for ABP. That's why we try to restore frame counters from
     * session information after a disconnection.
     *
     * @return    For ABP:  If everything goes well, LORAWAN_STATUS_OK is returned for first call followed by
     *                      a 'CONNECTED' event. Otherwise a negative error code is returned.
     *                      Any subsequent call will return LORAWAN_STATUS_ALREADY_CONNECTED and no event follows.
     *
     *            For OTAA: When a JoinRequest is sent, LORAWAN_STATUS_CONNECT_IN_PROGRESS is returned for the first call.
     *                      Any subsequent call will return either LORAWAN_STATUS_BUSY (if the previous request for connection
     *                      is still underway) or LORAWAN_STATUS_ALREADY_CONNECTED (if a network was already joined successfully).
     *                      A 'CONNECTED' event is sent to the application when the JoinAccept is received.
     */
    virtual lorawan_status_t connect();

    /** Connect OTAA or ABP with parameters
     *
     * All connection parameters are chosen by the user and provided in the
     * data structure passed down.
     *
     * When connecting via OTAA, the return code for success (LORAWAN_STATUS_CONNECT_IN_PROGRESS) is negative.
     * However, this is not a real error. It tells you that connection is in progress and you will
     * be notified of completion via an event. By default, after Join Accept message
     * is received, base stations may provide the node with a CF-List which replaces
     * all user-configured channels except the Join/Default channels. A CF-List can
     * configure a maximum of five channels other than the default channels.
     *
     * To configure more channels, we recommend that you use the `set_channel_plan()` API after the connection.
     * By default, the PHY layers configure only the mandatory Join
     * channels. The retransmission back-off restrictions on these channels
     * are severe and you may experience long delays or even
     * failures in the confirmed traffic. If you add more channels, the aggregated duty
     * cycle becomes much more relaxed as compared to the Join (default) channels only.
     *
     * **NOTES ON RECONNECTION:**
     * Currently, the Mbed OS LoRaWAN implementation does not support non-volatile
     * memory storage. Therefore, the state and frame counters cannot be restored after
     * a power cycle. However, if you use the `disconnect()` API to shut down the LoRaWAN
     * protocol, the state and frame counters are saved. Connecting again would try to
     * restore the previous session. According to the LoRaWAN 1.0.2 specification, the frame counters are always reset
     * to zero for OTAA and a new Join request lets the network server know
     * that the counters need a reset. The same is said about the ABP but there
     * is no way to convey this information to the network server. For a network
     * server, an ABP device is always connected. That's why storing the frame counters
     * is important, at least for ABP. That's why we try to restore frame counters from
     * session information after a disconnection.
     *
     * @param connect  Options for an end device connection to the gateway.
     *
     * @return    For ABP:  If everything goes well, LORAWAN_STATUS_OK is returned for first call followed by
     *                      a 'CONNECTED' event. Otherwise a negative error code is returned.
     *                      Any subsequent call will return LORAWAN_STATUS_ALREADY_CONNECTED and no event follows.
     *
     *            For OTAA: When a JoinRequest is sent, LORAWAN_STATUS_CONNECT_IN_PROGRESS is returned for the first call.
     *                      Any subsequent call will return either LORAWAN_STATUS_BUSY (if the previous request for connection
     *                      is still underway) or LORAWAN_STATUS_ALREADY_CONNECTED (if a network was already joined successfully).
     *                      A 'CONNECTED' event is sent to the application when the JoinAccept is received.
     */
    virtual lorawan_status_t connect(const lorawan_connect_t &connect);

    /** Disconnect the current session.
     *
     * @return         LORAWAN_STATUS_DEVICE_OFF on successfully shutdown.
     */
    virtual lorawan_status_t disconnect();

    /** Validate the connectivity with the network.
     *
     * Application may use this API to submit a request to the stack for
     * validation of its connectivity to a Network Server. Under the hood, this
     * API schedules a Link Check Request command (LinkCheckReq) for the network
     * server and once the response, i.e., LinkCheckAns MAC command is received
     * from the Network Server, user provided method is called.
     *
     * One way to use this API may be the validation of connectivity after a long
     * deep sleep. Mbed LoRaWANStack piggy-backs the MAC commands with data
     * frame payload so the application needs to try sending something and the Network
     * Server may respond during the RX slots.
     *
     * This API is usable only when the 'link_check_resp' callback is set by
     * the application. See add_lora_app_callbacks API. If the above mentioned
     * callback is not set, a LORAWAN_STATUS_PARAMETER_INVALID error is thrown.
     *
     * First parameter to callback function is the demodulation margin and
     * the second parameter is the number of gateways that successfully received
     * the last request.
     *
     * A 'Link Check Request' MAC command remains set for every subsequent
     * transmission, until/unless application explicitly turns it off using
     * remove_link_check_request() API.
     *
     * @return          LORAWAN_STATUS_OK on successfully queuing a request, or
     *                  a negative error code on failure.
     *
     */
    virtual lorawan_status_t add_link_check_request();

    /** Removes link check request sticky MAC command.
     *
     * Any already queued request may still get entertained. However, no new
     * requests will be made.
     */
    virtual void remove_link_check_request();

    /** Sets up a particular data rate
     *
     * `set_datarate()` first verifies whether the data rate given is valid or not.
     * If it is valid, the system sets the given data rate to the channel.
     *
     * @param data_rate   The intended data rate, for example DR_0 or DR_1.
     *                    Please note, that the macro DR_* can mean different
     *                    things in different regions.
     * @return            LORAWAN_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    virtual lorawan_status_t set_datarate(uint8_t data_rate);

    /** Enables adaptive data rate (ADR).
     *
     * The underlying LoRaPHY and LoRaMac layers handle the data rate automatically
     * for the user, based upon the radio conditions (network congestion).
     *
     * @return          LORAWAN_STATUS_OK or negative error code otherwise.
     */
    virtual lorawan_status_t enable_adaptive_datarate();

    /** Disables adaptive data rate.
     *
     * When adaptive data rate (ADR) is disabled, you can either set a certain
     * data rate or the MAC layer selects a default value.
     *
     * @return          LORAWAN_STATUS_OK or negative error code otherwise.
     */
    virtual lorawan_status_t disable_adaptive_datarate();

    /** Sets up the retry counter for confirmed messages.
     *
     * Valid for confirmed messages only.
     *
     * The number of trials to transmit the frame, if the LoRaMAC layer did not
     * receive an acknowledgment. The MAC performs a data rate adaptation as in
     * the LoRaWAN Specification V1.0.2, chapter 18.4, table on page 64.
     *
     * Note, that if number of retries is set to 1 or 2, MAC will not decrease
     * the datarate, if the LoRaMAC layer did not receive an acknowledgment.
     *
     * @param count     The number of retries for confirmed messages.
     *
     * @return          LORAWAN_STATUS_OK or a negative error code.
     */
    virtual lorawan_status_t set_confirmed_msg_retries(uint8_t count);

    /** Sets the channel plan.
     *
     * You can provide a list of channels with appropriate parameters filled
     * in. However, this list is not absolute. The stack applies a CF-List whenever
     * available, which means that the network can overwrite your channel
     * frequency settings right after Join Accept is received. You may try
     * to set up any channel or channels after that, and if the channel requested
     * is already active, the request is silently ignored. A negative error
     * code is returned if there is any problem with parameters.
     *
     * Please note that this API can also be used to add a single channel to the
     * existing channel plan.
     *
     * There is no reverse mechanism in the 1.0.2 specification for a node to request
     * a particular channel. Only the network server can initiate such a request.
     * You need to ensure that the corresponding base station supports the channel or channels being added.
     *
     * If your list includes a default channel (a channel where Join Requests
     * are received) you cannot fully configure the channel parameters.
     * Either leave the channel settings to default or check your
     * corresponding PHY layer implementation. For example, LoRaPHYE868.
     *
     * @param channel_plan      The channel plan to set.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t set_channel_plan(const lorawan_channelplan_t &channel_plan);

    /** Gets the channel plans from the LoRa stack.
     *
     * Once you have selected a particular PHY layer, a set of channels
     * is automatically activated. Right after connecting, you can use this API
     * to see the current plan. Otherwise, this API returns the channel
     * plan that you have set using `set_channel_plan()`.
     *
     * @param  channel_plan     The current channel plan information.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t get_channel_plan(lorawan_channelplan_t &channel_plan);

    /** Removes an active channel plan.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     * When a plan is abolished, only the non-default channels are removed.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t remove_channel_plan();

    /** Removes a single channel.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     *
     * @param    index          The channel index.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t remove_channel(uint8_t index);

    /** Send message to gateway
     *
     * @param port              The application port number. Port numbers 0 and 224
     *                          are reserved, whereas port numbers from 1 to 223
     *                          (0x01 to 0xDF) are valid port numbers.
     *                          Anything out of this range is illegal.
     *
     * @param data              A pointer to the data being sent. The ownership of the
     *                          buffer is not transferred. The data is copied to the
     *                          internal buffers.
     *
     * @param length            The size of data in bytes.
     *
     * @param flags             A flag used to determine what type of
     *                          message is being sent, for example:
     *
     *                          MSG_UNCONFIRMED_FLAG = 0x01
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *
     *                          All flags are mutually exclusive, and MSG_MULTICAST_FLAG
     *                          cannot be set.
     *
     *
     * @return                  The number of bytes sent, or
     *                          LORAWAN_STATUS_WOULD_BLOCK if another TX is
     *                          ongoing, or a negative error code on failure.
     */
    virtual int16_t send(uint8_t port, const uint8_t *data, uint16_t length,
                         int flags);

    /** Receives a message from the Network Server on a specific port.
     *
     * @param port              The application port number. Port numbers 0 and 224
     *                          are reserved, whereas port numbers from 1 to 223
     *                          (0x01 to 0xDF) are valid port numbers.
     *                          Anything out of this range is illegal.
     *
     * @param data              A pointer to buffer where the received data will be
     *                          stored.
     *
     * @param length            The size of data in bytes
     *
     * @param flags             A flag is used to determine what type of
     *                          message is being sent, for example:
     *
     *                          MSG_UNCONFIRMED_FLAG = 0x01,
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04,
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *
     *                          All flags can be used in conjunction with
     *                          one another depending on the intended use case or reception
     *                          expectation.
     *
     *                          e.g., MSG_CONFIRMED_FLAG and MSG_UNCONFIRMED_FLAG are
     *                          not mutually exclusive, i.e., the user can subscribe to
     *                          receive both CONFIRMED AND UNCONFIRMED messages at
     *                          the same time.
     *
     * @return                  It could be one of these:
     *                             i)   0 if there is nothing else to read.
     *                             ii)  Number of bytes written to user buffer.
     *                             iii) LORAWAN_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t port, uint8_t *data, uint16_t length, int flags);

    /** Receives a message from the Network Server on any port.
     *
     * @param data              A pointer to buffer where the received data will be
     *                          stored.
     *
     * @param length            The size of data in bytes
     *
     * @param port              Return the number of port to which message was received.
     *
     * @param flags             Return flags to determine what type of message was received.
     *                          MSG_UNCONFIRMED_FLAG = 0x01
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *
     * @return                  It could be one of these:
     *                             i)   0 if there is nothing else to read.
     *                             ii)  Number of bytes written to user buffer.
     *                             iii) LORAWAN_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t *data, uint16_t length, uint8_t &port, int &flags);

    /** Add application callbacks to the stack.
     *
     * An example of using this API with a latch onto 'lorawan_events' could be:
     *
     * LoRaWANInterface lorawan(radio);
     * lorawan_app_callbacks_t cbs;
     * static void my_event_handler();
     *
     * int main()
     * {
     * lorawan.initialize();
     *  cbs.lorawan_events = mbed::callback(my_event_handler);
     *  lorawan.add_app_callbacks(&cbs);
     *  lorawan.connect();
     * }
     *
     * static void my_event_handler(lorawan_event_t event)
     * {
     *  switch(event) {
     *      case CONNECTED:
     *          //do something
     *          break;
     *      case DISCONNECTED:
     *          //do something
     *          break;
     *      case TX_DONE:
     *          //do something
     *          break;
     *      default:
     *          break;
     *  }
     * }
     *
     * @param callbacks         A pointer to the structure containing application
     *                          callbacks.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t add_app_callbacks(lorawan_app_callbacks_t *callbacks);

    /** Change device class
     *
     * Change current device class.
     *
     * @param    device_class   The device class
     *
     * @return                  LORAWAN_STATUS_OK on success,
     *                          LORAWAN_STATUS_UNSUPPORTED is requested class is not supported,
     *                          or other negative error code if request failed.
     */
    virtual lorawan_status_t set_device_class(const device_class_t device_class);

    /** Get hold of TX meta-data
     *
     * Use this method to acquire any TX meta-data related to previous
     * transmission.
     * TX meta-data is only available right after the transmission is completed.
     * In other words, you can check for TX meta-data right after receiving the
     * TX_DONE event.
     *
     * @param    metadata    the inbound structure that will be filled if the meta-data
     *                       is available.
     *
     * @return               LORAWAN_STATUS_OK if the meta-data is available, otherwise
     *                       LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     */
    virtual lorawan_status_t get_tx_metadata(lorawan_tx_metadata &metadata);

    /** Get hold of RX meta-data
     *
     * Use this method to acquire any RX meta-data related to current
     * reception.
     * RX meta-data is only available right after the reception is completed.
     * In other words, you can check for RX meta-data right after receiving the
     * RX_DONE event.
     *
     * @param    metadata    the inbound structure that will be filled if the meta-data
     *                       is available.
     *
     * @return               LORAWAN_STATUS_OK if the meta-data is available, otherwise
     *                       LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     */
    virtual lorawan_status_t get_rx_metadata(lorawan_rx_metadata &metadata);

    /** Get hold of backoff time
     *
     * In the TX path, because of automatic duty cycling, the transmission is delayed
     * by a certain amount of time which is the backoff time. While the system schedules
     * application data to be sent, the application can inquire about how much time is
     * left in the actual transmission to happen.
     *
     * The system will provide you with a backoff time only if the application data is
     * in the TX pipe. If however, the event is already queued for the transmission, this
     * API returns a LORAWAN_STATUS_METADATA_NOT_AVAILABLE error code.
     *
     * @param    backoff    the inbound integer that will be carry the backoff time if it
     *                      is available.
     *
     * @return              LORAWAN_STATUS_OK if the meta-data regarding backoff is available,
     *                      otherwise LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     *
     */
    virtual lorawan_status_t get_backoff_metadata(int &backoff);

    /** Cancel outgoing transmission
     *
     * This API is used to cancel any outstanding transmission in the TX pipe.
     * If an event for transmission is not already queued at the end of backoff timer,
     * the system can cancel the outstanding outgoing packet. Otherwise, the system is
     * busy sending and can't be held back. The system will not try to re-send if the
     * outgoing message was a CONFIRMED message even if the ack is not received.
     *
     * @return              LORAWAN_STATUS_OK if the sending is cancelled.
     *                      LORAWAN_STATUS_BUSY otherwise.
     *
     */
    virtual lorawan_status_t cancel_sending(void);

    void lock(void)
    {
        _lw_stack.lock();
    }
    void unlock(void)
    {
        _lw_stack.unlock();
    }


private:
    typedef mbed::ScopedLock<LoRaWANInterface> Lock;

    LoRaWANStack _lw_stack;
};

#endif /* LORAWANINTERFACE_H_ */
