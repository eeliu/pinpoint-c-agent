--TEST--
app info 
--SKIPIF--
<?php if (!extension_loaded("pinpoint_php")) print "skip"; ?>
--INI--
pinpoint_php.CollectorHost=unix:/unexist_file.sock
pinpoint_php.SendSpanTimeOutMs=0
pinpoint_php.UnitTest=true
pinpoint_php.DebugReport=true
--FILE--
<?php 
var_dump(pinpoint_app_name());
var_dump(pinpoint_app_id());
var_dump(pinpoint_start_time());
var_dump(pinpoint_app_id().'^'.strval(pinpoint_start_time()).'^'.strval(pinpoint_unique_id()));
--EXPECTF--
[pinpoint] [%d] [%d]agent status changed: 2 ->2
string(18) "collector_blocking"
string(18) "collector_blocking"
int(%d)
string(%d) "collector_blocking^%d^%d"