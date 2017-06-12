<?php
defined("BASEPATH") or exit("No direct script access allowed");

class ACP_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('ACP_model', 'mod');
    }

    /**
     * The main method that charges admin panel.
     */
    public function admin($section = FALSE) {
        echo $section ? $this->templates->render('main::admin_' . $section, ['sess' => $this])
                : $this->templates->render('main::admin', ['sess' => $this]);
    }

    /**
     * This method is used for ajax query in the ACP.
     */
    public function roms_list() {
        $data = $this->mod->get_roms();
        echo json_encode($data);
    }
}
