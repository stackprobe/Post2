﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte
{
	public interface IService
	{
		object Perform(object prm);
		//void DiskYellow(); // del @ 2020.4.1
	}
}
