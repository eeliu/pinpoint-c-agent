<?php
#-------------------------------------------------------------------------------
# Copyright 2019 NAVER Corp
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy
# of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#-------------------------------------------------------------------------------

/**
 * User: eeliu
 * Date: 3/29/19
 * Time: 3:40 PM
 */

namespace Plugins;
use Plugins\Candy;

///@hook:app\TestPDO::\PDO::query app\TestPDO::\PDO::prepare
///@hook:app\TestPDO::\PDO::__construct
///@hook:app\TestMysqli::\mysqli::query app\TestMysqli::\mysqli::__construct
class PDOCommonPlugin extends Candy
{
    function onBefore()
    {
        pinpoint_add_clue("stp",MYSQL);
        pinpoint_add_clues(PHP_ARGS,print_r($this->args,true));
        pinpoint_add_clue("dst","MysqlHost");

    }
    function onEnd(&$ret)
    {
        echo "ret:";
        var_dump($ret);
        pinpoint_add_clues(PHP_RETURN, print_r($ret,true));
    }

    function onException($e)
    {
        pinpoint_add_clue("EXP",$e->getMessage());
    }
}
