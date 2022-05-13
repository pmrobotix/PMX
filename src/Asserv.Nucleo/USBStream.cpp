//#include "USBStream.h"
//#include "usbcfg.h"
//#include <ch.h>
//#include <hal.h>
//#include "core_cm4.h"
//#include <cstring>
//
//const uint32_t synchroWord_stream = 0xCAFED00D;
//const uint32_t synchroWord_config = 0xCAFEDECA;
//
//USBStream *USBStream::s_instance = NULL;
//USBStream::USBStream()
//{
//    m_currentPtr = NULL;
//    m_timestamp = 0;
//    m_bufferSize = 0;
//    std::memset(&m_currentStruct, 0xFFFFFFFF, sizeof(m_currentStruct));
//}
//
//void USBStream::init()
//{
//    // USB FS
//    palSetPadMode(GPIOA, 12, PAL_MODE_ALTERNATE(10)); //USB D+
//    palSetPadMode(GPIOA, 11, PAL_MODE_ALTERNATE(10)); //USB D-
//
//    s_instance = new USBStream();
//
//    /*
//     * Initializes a serial-over-USB CDC driver.
//     */
//    sduObjectInit(&SDU1);
//    std::memset(SDU1.ob, 0x0F0F0F0F, sizeof(SDU1.ob));
//    sduStart(&SDU1, &serusbcfg);
//
//    /*
//     * Activates the USB driver and then the USB bus pull-up on D+.
//     * Note, a delay is inserted in order to not have to disconnect the cable
//     * after a reset.
//     */
//    usbDisconnectBus(serusbcfg.usbp);
//    chThdSleepMilliseconds(1000);
//    usbStart(serusbcfg.usbp, &usbcfg);
//    usbConnectBus(serusbcfg.usbp);
//
//    s_instance->getEmptyBuffer();
//
//}
//
//void* USBStream::sendCurrentStream()
//{
//    m_currentStruct.synchro = synchroWord_stream;
//    m_currentStruct.timestamp = m_timestamp++;
//
//    sendFullBuffer();
//
//    getEmptyBuffer();
//
//    return m_currentPtr;
//}
//
//void USBStream::sendConfig(uint8_t *configBuffer, uint8_t size)
//{
//    chnWrite(&SDU1, (uint8_t*)&synchroWord_config, sizeof(synchroWord_config));
//    chnWrite(&SDU1, &size, sizeof(size));
//    chnWrite(&SDU1, configBuffer, size);
//}
//
//void USBStream::sendFullBuffer()
//{
//    if (m_currentPtr != NULL) {
//        *m_currentPtr = m_currentStruct;
//        obqPostFullBuffer(&SDU1.obqueue, sizeof(UsbStreamSample));
//    }
//}
//
//void USBStream::getEmptyBuffer()
//{
//    msg_t msg = obqGetEmptyBufferTimeout(&SDU1.obqueue, 0);
//    if (msg == MSG_OK) {
//        m_currentPtr = (UsbStreamSample*) SDU1.obqueue.ptr;
//        uint32_t available_size = ((uint32_t) SDU1.obqueue.top - (uint32_t) SDU1.obqueue.ptr);
//        chDbgAssert(available_size >= (sizeof(UsbStreamSample) + 4),
//                "Not enough space in the free buffer. Did you set a correct USB buffer size ?");
//    } else {
//        m_currentPtr = NULL;
//    }
//}
//
//void USBStream::getFullBuffer(void **ptr, uint32_t *size)
//{
//    *size = 0;
//    if (!is_usb_serial_configured()) {
//        /*
//         *  Workaround :
//         *   When USB isn't plugged, ibqGetFullBufferTimeout return MSG_RESET immediately and this thread stuck others threads.
//         *   is_usb_serial_configured return the number of time the usb serial driver was configured ( ie : plugged in a PC ),
//         *    so while this condition isn't true, just schedule the others thread.
//         */
//
//        chThdYield();
//    }
//
//    msg_t msg = ibqGetFullBufferTimeout(&SDU1.ibqueue, TIME_INFINITE);
//
//    uint32_t sizeToRead = (size_t) SDU1.ibqueue.top - (size_t) SDU1.ibqueue.ptr;
//    if (msg != MSG_OK || sizeToRead == 0)
//        return;
//
//    *size = sizeToRead;
//    *ptr = SDU1.ibqueue.ptr;
//}
//
//void USBStream::releaseBuffer()
//{
//    ibqReleaseEmptyBuffer(&SDU1.ibqueue);
//}
