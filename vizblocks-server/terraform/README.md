# Terraform

Terraform is an infrastructure management tool. You can use it to create virtual machines, security groups etc in OpenStack, so it's a natural tool to use here to describe the infrastructure on which we want to host our application.

Nb you will need to create `clouds.yaml` and `private.auto.tfvars` in this directory to use this confg. See the main README for further details.

# Openstack CLI

Download and source your OpenStack RC file in the current enviroment. e.g. `source ECA-Research-DevTest-openrc.sh`

Helpful commands:
- `openstack flavor list`

- `openstack image list`

- `openstack security group list`

# Useful stuff

Once the instance is set up you can ssh into the VM and follow the cloud-init logs with `tail -f /var/log/cloud-init-output.log `