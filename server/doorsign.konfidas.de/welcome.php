<?php
$fp = fopen('state.txt', 'w');
fwrite($fp, 'welcome');
fclose($fp);
http_response_code(200);
?>
