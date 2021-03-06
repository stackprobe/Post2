﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class ServiceDistributor
	{
		private class ServiceInfo
		{
			public string Command;
			public IService Service;
		}

		private List<ServiceInfo> Infos = new List<ServiceInfo>();

		public ServiceDistributor()
		{
			// ---- 共通 ----

			this.Add("save", new Services.SaveLoad.Service.Save());
			this.Add("load", new Services.SaveLoad.Service.Load());

			// ---- ページ固有 ----

			this.Add("index", new Services.Index.Service());
			this.Add("salad/uploader", new Services.Salad.Uploader.Service());
			this.Add("test-service/echo", new Services.TestService.Echo.Service());

			// ----
		}

		private void Add(string command, IService service)
		{
			this.Infos.Add(new ServiceInfo()
			{
				Command = command,
				Service = service,
			});
		}

		public IService GetService(string command)
		{
			return this.Infos.First(v => StringTools.CompIgnoreCase(v.Command, command) == 0).Service;
		}

		public IEnumerable<IService> GetAllService()
		{
			return this.Infos.Select(v => v.Service);
		}
	}
}
