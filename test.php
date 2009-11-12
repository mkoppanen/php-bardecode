<?php

dl("bardecode.so");

$b = new bardecode();
var_dump($b->setParameter(bardecode::BARCODE_ZONE, array(1, 1, 22, 22)));
var_dump($b->getParameter(bardecode::BARCODE_ZONE));
//var_dump($b->read("test.jpg"));
