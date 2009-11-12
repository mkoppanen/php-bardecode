--TEST--
Read a barcode
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
$bardecode = new bardecode();
$bardecode->setParameter(bardecode::READ_EAN13, true);
var_dump($bardecode->read(dirname(__FILE__) . "/ean13.jpg"));
?>
--EXPECTF--
EVALUATION COPY (does not display last 3 characters of a barcode)
If you would like a full version to evaluate then please contact sales@bardecode.com.
Copyright 2002 - 2009 Softek Software Limited
array(1) {
  [0]=>
  array(5) {
    ["code"]=>
    string(12) "590123412???"
    ["type"]=>
    string(5) "EAN13"
    ["confidence"]=>
    int(6)
    ["direction"]=>
    int(1)
    ["coordinates"]=>
    array(5) {
      ["page"]=>
      int(1)
      ["top_left_x"]=>
      int(64)
      ["top_left_y"]=>
      int(291)
      ["bottom_right_x"]=>
      int(434)
      ["bottom_right_y"]=>
      int(293)
    }
  }
}