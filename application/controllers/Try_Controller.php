<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Try_Controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
    }

    public function index() {
        echo $this->templates->render('home', ['name' => 'Cheke']);
    }

    public function inheritance() {
        echo $this->templates->render('inheritance', ['name' => 'Cheke']);
    }

}
