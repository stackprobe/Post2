using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Services.TestService.Echo
{
	public class Service : IService
	{
		public object Perform(object prm)
		{
			return prm;
		}

		public void DiskYellow()
		{
			// noop
		}
	}
}
