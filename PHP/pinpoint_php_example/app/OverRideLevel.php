<?php


namespace app;

#app\OverRideLevel::public_function app\OverRideLevel::protected_function function app\OverRideLevel::private_function
class OverRideLevel extends LevelClass
{
    protected function protected_function($a)
    {
        parent::protected_function(); // TODO: Change the autogenerated stub
        echo "Protected func in subclass";
        return $a;
    }

    public function public_function($a)
    {
        parent::public_function(); // TODO: Change the autogenerated stub
        echo "Public func in subclass";
        return $a;
    }
}