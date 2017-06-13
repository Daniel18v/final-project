<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Consoles_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('emulators_model', 'mod');
    }

    /**
     * Makes a query to obtain all roms, in order to list them and show in the correct view.
     *
     * @param mixed $type Contains the games' platform in order to obtain all roms of it.
     */
    public function emulator($type = FALSE) {
        $roms = $this->mod->get_roms($type);
        empty(!$roms) ? : show_404();
        echo $this->templates->render('main::platform', ['sess' => $this, 'roms' => $roms]);
    }

}