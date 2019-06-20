<?php
$state = file_get_contents('state.txt');

if ($state=="welcome"){
    $return = file_get_contents('welcome.img');
}

if ($state=="stop"){
    $return = file_get_contents('stop.img');
}
echo $return;

?>
