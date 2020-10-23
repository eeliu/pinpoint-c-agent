<?php
namespace Plugins\Sys\curl;

static $chArr= [];

function curl_init($url = null)
{
    $ch = \curl_init($url);
    global $chArr;
    if($url){
        $chArr[strval($ch)] = ['url'=>$url];

    }else{
        $chArr[strval($ch)] = [];
    }
    return $ch;
}


function curl_setopt($ch, $option, $value)
{
    if($option == CURLOPT_HTTPHEADER)
    {
        CurlUtil::appendPinpointHeader($value);
    }elseif ($option == CURLOPT_URL ){
        CurlUtil::appendPinpointHeader($value);
    }

    return \curl_setopt($ch, $option, $value);
}
function curl_setopt_array($ch, array $options)
{
    $args = \pinpoint_get_func_ref_args();
    $commPlugins_curl_setopt_array_var = new NextSpanPlugin('curl_setopt_array', null, $args);
    try {
        $commPlugins_curl_setopt_array_var->onBefore();
        $commPlugins_curl_setopt_array_ret = call_user_func_array('curl_setopt_array', $args);
        $commPlugins_curl_setopt_array_var->onEnd($commPlugins_curl_setopt_array_ret);
        return $commPlugins_curl_setopt_array_ret;
    } catch (\Exception $e) {
        $commPlugins_curl_setopt_array_var->onException($e);
        throw $e;
    }
}
function curl_exec($ch)
{
    $args = \pinpoint_get_func_ref_args();
    $commPlugins_curl_exec_var = new NextSpanPlugin('curl_exec', null, $args);
    try {
        $commPlugins_curl_exec_var->onBefore();
        $commPlugins_curl_exec_ret = call_user_func_array('curl_exec', $args);
        $commPlugins_curl_exec_var->onEnd($commPlugins_curl_exec_ret);
        return $commPlugins_curl_exec_ret;
    } catch (\Exception $e) {
        $commPlugins_curl_exec_var->onException($e);
        throw $e;
    }
}

function curl_close($ch)
{
    global $chArr;
    unset($chArr[strval($ch)] );
    \curl_close($ch);
}

function curl_reset($ch)
{
    global $ch_arr;
    $ch_arr[strval($ch)] = [];
    \curl_reset($ch);
}
