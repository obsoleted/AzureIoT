// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SIMPLESAMPLEHTTP_H
#define SIMPLESAMPLEHTTP_H

#ifdef __cplusplus
extern "C" {
#endif
    bool init_azureiot_hub(const char* connectionString);
    bool cleanup_azureiot_hub();
    bool send_event(unsigned char* message, size_t messageSize);
    bool register_azureiot_model(void* model);
    void azureiot_dowork();
#ifdef __cplusplus
}
#endif

#endif /* SIMPLESAMPLEHTTP_H */
