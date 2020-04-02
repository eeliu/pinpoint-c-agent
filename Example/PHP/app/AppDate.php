<?php


namespace app;


class AppDate
{
    static function consumerCPU($i)
    {
        $ret  = 0;
        for ($t =0 ;$t<$i;$t++)
        {
            for($a = 0;$a<$t;$a++)
            {
                $ret+=$a;
            }
        }
        return $ret;
    }
    static function outputDate(){
        echo static::consumerCPU(1000).'<br>';
        echo date("Y/m/d")."<br>";
        echo getenv('REMOTE_ADDR')."<br>";
        echo getenv('HTTP_HOST')."<br>";
        echo getenv('HTTP_X_FORWARDED_FOR')."<br>";
        echo getenv('HTTP_X_FORWARDED_HOST')."<br>";
        echo getenv('HTTP_X_FORWARDED_SERVER')."<br>";
//        throw new \Exception("I want throw something");
//        print_r(ini_get_all('pinpoint_php'));
//        echo ini_get('pinpoint_php.CollectorHost')."\n";
//        echo ini_get('pinpoint_php.SendSpanTimeOutMs')."\n";
//        var_dump(ini_get('pinpoint_php.UnitTest"'));
//
//        echo ini_get('pinpoint_php.AppId')."\n";
//
//        echo ini_get('pinpoint_php.StartTime');
//        print_r($_SERVER);
    }
}