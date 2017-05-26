<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Roms_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('emulators_model', 'mdl');
    }

    public function emulator($type = FALSE, $slug = FALSE) {
        $query   = $this->mdl->get_rom($type, $slug);
        $romName = $query[0]->title;
        echo $this->templates->render('emulator::js'.$type, ['sess' => $this, 'rom' => $romName]);
    }
}