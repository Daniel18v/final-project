<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Consoles_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('emulators_model', 'mdl');
    }

    public function emulator($type = FALSE) {
        $roms = $this->mdl->get_roms($type);
        echo $this->templates->render('main::platform', ['sess' => $this, 'roms' => $roms]);
    }

}