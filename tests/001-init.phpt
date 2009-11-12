--TEST--
Test construction
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
$bardecode = new bardecode();
echo "OK";
?>
--EXPECT--
OK