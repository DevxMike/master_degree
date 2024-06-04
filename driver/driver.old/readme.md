# how to run docker image
	Make sure that you installed docker on your system!
		$ docker-compose up -d
		$ Starting driver_driver_1 ... done
		$ docker ps
		$ 8ca53468d1c0   osrf/ros:foxy-desktop   "/ros_entrypoint.sh …"   6 minutes ago   Up 18 seconds             driver_driver_1
		$ docker exec -it driver_driver_1 bash
	Now we are inside docker image. If you need to pull ros2 image:
		$ docker pull osrf/ros:foxy-desktop
	Or refer to documentation:
		https://docs.ros.org/en/foxy/How-To-Guides/Run-2-nodes-in-single-or-separate-docker-containers.html
