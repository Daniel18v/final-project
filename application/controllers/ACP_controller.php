<?php
defined("BASEPATH") or exit("No direct script access allowed");

class ACP_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
    }

    public function admin() {
        echo $this->templates->render('main::admin',['sess' => $this]);
    }

}