<?php
/******************************************************************************
 * Copyright 2020 NAVER Corp.                                                 *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/
namespace Plugins\Framework\Swoole\curl;

use Plugins\Framework\Swoole\IDContext;
use Plugins\Util\Trace;
use Plugins\Sys\curl\CurlUtil as URLUtils;
class CurlUtil
{
    public static function getPinpointHeader()
    {
        if(pinpoint_get_context('Pinpoint-Sampled',IDContext::get())==PP_NOT_SAMPLED){
            return ["Pinpoint-Sampled:s0"];
        }

        $nsid = Trace::generateSpanID();
        $header = [
            'Pinpoint-Sampled:s1',
            'Pinpoint-Flags:0',
            'Pinpoint-Papptype:1500',
            'Pinpoint-Pappname:'.APPLICATION_NAME,
            'Pinpoint-Host:'.gethostname(),
            'Pinpoint-Traceid:'.pinpoint_get_context(PP_TRANSCATION_ID,IDContext::get()),
            'Pinpoint-Pspanid:'.pinpoint_get_context(PP_SPAN_ID,IDContext::get()),
            'Pinpoint-Spanid:'.$nsid
        ];
        pinpoint_set_context(PP_NEXT_SPAN_ID, (string)$nsid,IDContext::get());
        return $header;
    }


}