provider "openstack" {
   cloud = "openstack" 
}

variable "vizblocks_server" {
   type = map
}

resource "openstack_compute_instance_v2" "vizblocks_server" {
  name            = "vizblocks-server"
  image_id        = "0e935645-97c2-4013-8950-ba939638c2d9"
  flavor_id       = "3"
  key_pair        = "mykey"
  security_groups = ["default", "webserver", "mqtt"]
  user_data = templatefile("cloud-config-vizblocks-server.yml", var.vizblocks_server)  

  metadata = {
    this = "that"
  }

  network {
    name = "VM Network Public"
  }
}

resource "openstack_compute_floatingip_associate_v2" "vizblocks_server_ip" {
   floating_ip = "129.215.193.113"
   instance_id = openstack_compute_instance_v2.vizblocks_server.id
   fixed_ip    = openstack_compute_instance_v2.vizblocks_server.network.0.fixed_ip_v4
}