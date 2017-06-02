<?php
defined("BASEPATH") or exit("No direct script access allowed");

class ACP_controller extends MY_Controller
{

    public function __construct() {
        parent::__construct();
        $this->load->model('ACP_model', 'roms');
    }
    public function admin() {
        echo $this->templates->render('main::admin', ['sess' => $this]);
    }
    public function roms_list() {
        $data = $this->roms->get_roms();
        echo json_encode($data);
    }
    public function prueba() {

    }
}
