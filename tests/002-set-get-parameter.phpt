--TEST--
Test setting and getting options
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
$bardecode = new bardecode();

$bardecode->setParameter(bardecode::READ_EAN13, true);
var_dump($bardecode->getParameter(bardecode::READ_EAN13));

$bardecode->setParameter(bardecode::READ_EAN13, false);
var_dump($bardecode->getParameter(bardecode::READ_EAN13));

$bardecode->setParameter(bardecode::REGEX_MATCH, "[a-zA-Z]");
var_dump($bardecode->getParameter(bardecode::REGEX_MATCH));

$bardecode->setParameter(bardecode::CONTRAST, 100);
var_dump($bardecode->getParameter(bardecode::CONTRAST));

?>
--EXPECT--
bool(true)
bool(false)
string(8) "[a-zA-Z]"
int(100)