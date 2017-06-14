<?php

defined("BASEPATH") or exit("No direct script access allowed");

class ACP_users_model extends CI_Model {
    public function __construct() {
        $this->load->database();
    }

    public function get_users() {
        $query = $this->db->get("users");
        return $query->result_array();
    }

    public function insertUser($data) {
        $this->db->insert('users', $data);
    }

    public function deleteUser($data) {
        $this->db->where('id', $data['id']);
        $this->db->delete('users');
    }

    public function updateUser($data) {
        $this->db->where('id', $data['id']);
        $this->db->update('users', $data);
    }
}