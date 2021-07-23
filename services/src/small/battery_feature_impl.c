/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <ohos_errno.h>
#include <iproxy_server.h>
#include <pthread.h>
#include <unistd.h>

#include "hilog_wrapper.h"
#include "battery_manage_feature.h"

typedef int32_t (*InvokeFunc)(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);
static int32_t FeatureInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply);

static int32_t BatterySocInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);
static int32_t ChargingStatusInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);
static int32_t HealthStatusInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply);
static int32_t PluggedTypeInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);
static int32_t VoltageInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);
static int32_t TechnologyInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply);
static int32_t BatteryTemperatureInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply);

static BatteryFeatureApi g_feature = {
    BATTERY_FEATURE_INTERFACE_IMPL,
    SERVER_IPROXY_IMPL_BEGIN,
    .Invoke = FeatureInvoke,
    BATTERY_INTERFACE_IMPL,
    IPROXY_END,
    .identity_ = { -1, -1, NULL },
};

static InvokeFunc g_invokeFuncs[BATTERY_FUNCID_END] = {
    BatterySocInvoke,
    ChargingStatusInvoke,
    HealthStatusInvoke,
    PluggedTypeInvoke,
    VoltageInvoke,
    TechnologyInvoke,
    BatteryTemperatureInvoke
};

BatteryFeatureApi *GetBatteryFeatureImpl(void)
{
    return &g_feature;
}

static int32_t FeatureInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply)
{
    if ((iProxy == NULL) || (origin == NULL) || (req == NULL)) {
        return EC_INVALID;
    }
    return (funcId >= 0 && funcId < BATTERY_FUNCID_END) ? g_invokeFuncs[funcId](iProxy, origin, req, reply) :
        EC_FAILURE;
}

static int32_t BatterySocInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    int32_t ret = BatterySocImpl((IUnknown *)iProxy);
    IpcIoPushInt32(reply, ret);
    return EC_SUCCESS;
}
static int32_t ChargingStatusInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    BatteryChargeState status = ChargingStatusImpl((IUnknown *)iProxy);
    IpcIoPushInt32(reply, status);
    return EC_SUCCESS;
}
static int32_t HealthStatusInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    BatteryHealthState status = HealthStatusImpl((IUnknown *)iProxy);
    IpcIoPushInt32(reply, status);
    return EC_SUCCESS;
}
static int32_t PluggedTypeInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    BatteryPluggedType status = PluggedTypeImpl((IUnknown *)iProxy);
    IpcIoPushInt32(reply, status);
    return EC_SUCCESS;
}
static int32_t VoltageInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    int32_t ret = VoltageImpl((IUnknown *)iProxy);
    IpcIoPushInt32(reply, ret);
    return EC_SUCCESS;
}
static int32_t TechnologyInvoke(IServerProxy *iProxy, int32_t funcId, void *origin, IpcIo *req, IpcIo *reply)
{
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    char *strBuff = TechnologyImpl((IUnknown *)iProxy);
    IpcIoPushString(reply, strBuff);

    return EC_SUCCESS;
}
static int32_t BatteryTemperatureInvoke(IServerProxy *iProxy, void *origin, IpcIo *req, IpcIo *reply)
{
    int32_t ret;
    if (iProxy == NULL) {
        return EC_FAILURE;
    }

    ret = BatteryTemperatureImpl((IUnknown*)iProxy);
    IpcIoPushInt32(reply, ret);
    return EC_SUCCESS;
}