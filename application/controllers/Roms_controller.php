<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Roms_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('emulators_model', 'mod');
    }

    /**
     * Shows a game for a specific console.
     *
     * @param bool|string $type A specific console.
     * @param bool|string $slug A specific game.
     */
    public function emulator($type = FALSE, $slug = FALSE) {
        $query = $this->mod->get_rom($type, $slug);
        empty(!$query) ? $romName = $query[0] : show_404();
        echo $this->templates->render('emulator::js' . $type, ['sess' => $this, 'rom' => $romName]);
    }
}